/*------------------------------------------------------------------
 *  statemachine.c -- main state machine function and related code
 *
 *  I. Snehal Jauhri
 *  Embedded Software Lab
 *
 *  May 2019
 *------------------------------------------------------------------
 */

#include "in4073.h"
#include "statemachine.h"
#include "packet.h"
#include "messageDecoder.h"
#include "control.h"
#include "calibration.h"
#include "datalogging.h"
#include "filters.h"
#include "profiling.h"

/* global variable with current mode */
enum modes current_mode = safe;

/* Timekeeping */
static uint32_t prev_time_com = 0;
static uint32_t prev_time_log = 0;

/* End program */
static bool end_flag = false;

/*--------------------------------------------------------------------------------------
 * batteryCheck -- Check battery voltage and enter panic mode in case of empty battery
 *--------------------------------------------------------------------------------------
 */    
void batteryCheck()
{
    /* Only check if not in safe/panic */
    if (current_mode > panic)
    {
        if ((bat_volt < MIN_BATTERY) && (bat_volt > SOME_VOLTS))
        {
            current_mode = panic;
            nrf_gpio_pin_set(YELLOW);
            printf("The voltage is %d -> switching to panic\n",bat_volt);
        }
    }

    adc_request_sample();
}

/*------------------------------------------------------------------
 * send_flashdata -- sends the log data from flash to pc
 *------------------------------------------------------------------
 */
bool send_flashdata()
{
    if(prev_time_log == 0){
        prev_time_log = get_time_us();
    }
    if(get_time_us() >= (prev_time_log + 20000)){        
        uint8_t flashData[LOG_RPLY_SIZE-3];

        if (datalog_read(flashData)) {
            create_log_rply_pkt(&tx_queue, flashData);
            prev_time_log = 0;
            prev_time_com = 0;
            return true;
        } 
        else {
            prev_time_log = 0;
            prev_time_com = 0;
            return false;
        }
    }
    prev_time_com = 0;
    return true;
}

/*------------------------------------------------------------------
 * drone_fsm -- run overall state machine in current mode
 *------------------------------------------------------------------
 */
bool drone_fsm(void)
{    
    //startMeasurement(STATE_MACHINE_MEAS);
    /* Message decode and reaction */
    message_get();
    message_react();

    /* Tasks that only need to be done when in a flying mode */
    if((current_mode == manual) || (current_mode == yaw_ctrl) || (current_mode == full_ctrl) || (current_mode == height))
    {
        // Safety: Check if no messages received from PC for a long time. Switch to panic if this is the case.
        if(prev_time_com == 0) // Reset on first start of timing mechanism
        {
            prev_time_com = get_time_us();
        }        
        if((get_time_us() >= (prev_time_com + FIFTY_MS)))
        {
            if(flightdatacount == ZERO_FLIGHT_COUNT) // If no messages received, panic!
            {
                current_mode = panic;
            }
            flightdatacount = 0;
            prev_time_com = get_time_us();
        }
        
        // If datalog is enabled, log at maximum speed
        if(datalog_enable && get_time_us() >= (prev_time_log)){
            if(!datalog_write(current_mode)){
                printf("Flash full; Logging complete\n");
                datalog_enable = false;
                }
            prev_time_log = get_time_us();     
        }

    }
    else /* Tasks when not in a flying mode */
    {
        /* Read sensor to avoid fifo overflow */            
        if (check_sensor_int_flag())
        {     
            if(!dmp_used) // When using filters
            {
                get_raw_sensor_data();
                /* Reset filters to avoid errors from previous values */
                reset_filters();
            }
            else
            {   
                get_dmp_data();
            }
        }
        // Reset safety timing
        prev_time_com = 0;
    }

    switch(current_mode)
    {
        case safe:
            /* Ensure rotors at zero rpm */
            shutoff_motors();

            nrf_gpio_pin_set(RED);
            /* Check if mode change requested */            
            if(requested_mode > panic && requested_mode != height) /* No need to unnecesarily switch to panic from safe */
            {
                printf("Switching mode..  %d\n", requested_mode);
                current_mode = requested_mode;
            }        
            break;

        case panic:
            printf("\n \nPANIC! AAAAHHH!\n \n");
            nrf_gpio_pin_clear(RED);
            
            /* Set rotor speeds to a minimum if drone is already flying i.e. lift is non-trivial */
            if ((ae[0] > MIN_SPEED) || (ae[1] > MIN_SPEED) || (ae[2] > MIN_SPEED) || (ae[3] > MIN_SPEED))
            {
                map_lift(lift-LIFT_REDUCTION);
                update_motors();
                nrf_delay_ms(500); // Delay 0.5 second
                map_lift(lift-(2*LIFT_REDUCTION));
                update_motors();
                nrf_delay_ms(500); // Delay 0.5 second
                map_lift(lift-(3*LIFT_REDUCTION));
                update_motors();
                nrf_delay_ms(500); // Delay 0.5 second
                map_lift(lift-(4*LIFT_REDUCTION));
                update_motors();
                nrf_delay_ms(500); // Delay 0.5 second
                
            }
            else
            {
                shutoff_motors();
            }                                                           
            printf("Switching mode..  0\n");
            current_mode = safe;
            requested_mode = safe; // Reset
            break;

        case manual:                                    
            if(requested_mode < manual) /* If switching to safe/panic */
            {
                printf("Switching mode..  %d\n", requested_mode);
                current_mode = requested_mode;
                break;
            }        
            /* send rpm to motors */
            update_ae(roll, pitch, yaw, lift);
            update_motors();

            /* Read sensor to avoid fifo overflow */            
            if (check_sensor_int_flag())
            {     
                if(!dmp_used)
                {
                    get_raw_sensor_data();                
                }
                else
                {   
                    get_dmp_data();
                }
            }
            break;

        case calib:
            printf("Calibrating.....\n");
            static_calibration();
            printf("Calibration complete\n");
            /* Switch back to safe when done */
            printf("Switching mode..  0\n");
            current_mode = safe;
            requested_mode = safe; // Reset
            break;

        case yaw_ctrl:
            if(requested_mode < manual) /* If switching to safe/panic */
            {
                printf("Switching mode..  %d\n", requested_mode);
                current_mode = requested_mode;
                break;
            }                        
            /* Sensor Acq */
            yaw_sensoracq_and_filters();
            /* run control */
            run_yaw_control(roll, pitch, yaw, lift);
            break;

        case full_ctrl:                        
            if(requested_mode < manual) /* If switching to safe/panic */
            {
                printf("Switching mode..  %d\n", requested_mode);
                current_mode = requested_mode;
                break;
            }
            else if (requested_mode == height){
                printf("Switching mode..  %d\n", requested_mode);            
                read_baro_setpoint();
                current_mode = requested_mode;
                break;
            }            
            /* Sensor Acq */
            full_sensoracq_and_filters();
            /* run control */
            run_full_control(roll, pitch, yaw, lift);                                      
            break;

        case use_raw:
            printf("Switching to RAW data.....\n");
            dmp_used = false;
            imu_init(false, IMU_RAW_FREQ);            
            /* Switch back to safe when done */
            printf("Switching mode..  0\n");
            current_mode = safe;
            requested_mode = safe; // Reset
            break;
            
        case use_dmp:
            printf("Switching to DMP data.....\n");
            dmp_used = true;
            imu_init(true, IMU_DMP_FREQ);            
            /* Switch back to safe when done */
            printf("Switching mode..  0\n");
            current_mode = safe;
            requested_mode = safe; // Reset
            break;

        case height:
            if(requested_mode < manual || requested_mode == full_ctrl) /* If switching to safe/panic/full */
            {
                printf("Switching mode..  %d\n", requested_mode);
                current_mode = requested_mode;
                break;
            }
            /* Sensor Acq */
            hight_sensoracq_and_filters();

            if(lift_setpoint > (lift + 1000 )|| lift_setpoint < (lift - 1000)){
                printf("Lift changed -> switching mode..  %d\n", full_ctrl);
                requested_mode = full_ctrl;
                current_mode = full_ctrl;
                break;
            }
            /* run control */
            run_hight_control(roll, pitch, yaw);                                      
            break;
        case sendinglog:
            /* Stay in this mode until all log data is sent */
            if(!send_flashdata()){
                /* All data sent, send a final done packet */
                create_log_done_pkt(&tx_queue);
                /* Switch back to safe mode */
                printf("Switching mode..  0\n");
                current_mode = safe;
                requested_mode = safe; // Reset
            }
            break;
            
        default:
            /* Illegal state reached. switch to panic mode */
            current_mode = panic;
    }      

    //stopMeasurement(STATE_MACHINE_MEAS);
    return !end_flag;
}
