/*------------------------------------------------------------------
 *  filters.c -- Implementation for Butterworth and Kalman filters
 * 
 *  Embedded Software Lab
 *
 *  June 2019
 *
 * 	Authors: Snehal Jauhri
 *------------------------------------------------------------------
 */

#include "in4073.h"
#include "calibration.h"
#include "filters.h"
#include "profiling.h"
#include "datalogging.h"

/* Debug */
uint8_t biggestmeasurement = 0;

/* Reset the filter's (previous) values */
void reset_filters(void)
{
    phi = 0, theta = 0, psi = 0;
    p = 0, q = 0, r = 0;

    phi_fp = 0, theta_fp = 0, psi_fp = 0;
    p_fp = 0, q_fp = 0, r_fp = 0;
    
    sphi_x1_fp = 0, sphi_x2_fp = 0, stheta_x1_fp = 0, stheta_x2_fp = 0, spsi_x1_fp = 0, spsi_x2_fp = 0;
    sphi_y1_fp = 0, sphi_y2_fp = 0, stheta_y1_fp = 0, stheta_y2_fp = 0, spsi_y1_fp = 0, spsi_y2_fp = 0;
    sp_x1_fp = 0, sp_x2_fp = 0, sq_x1_fp = 0, sq_x2_fp = 0, sr_x1_fp = 0, sr_x2_fp = 0;
    sp_y1_fp = 0, sp_y2_fp = 0, sq_y1_fp = 0, sq_y2_fp = 0, sr_y1_fp = 0, sr_y2_fp = 0;
    
    p_b_fp = 0, q_b_fp = 0;
}

/* FIXED POINT mutiply */
int32_t fp_mult(int32_t x, int32_t y)
{
    int64_t res = ((int64_t)x) * ((int64_t)y);
    return (int32_t)(res >> FP_SHIFT_AMOUNT);
}

/* Function to correct DC offsets and run filter for Yaw rate. 
r must be set correctly at the end of this function */
void yaw_rate_filter(void)
{
    int16_t sr_corrected;
    int32_t sr_raw_fp, sr_filtered_fp;

    /* Filter only when not in dmp mode */
    if(!dmp_used)
    {
        // Correct DC offset
        sr_corrected = sr - sr_offset;
                        
        // Convert to fixed point
        sr_raw_fp = (sr_corrected << FP_SHIFT_AMOUNT);
        // Multiply butter params
        sr_filtered_fp = fp_mult(BUTTER_A0,sr_raw_fp) + fp_mult(BUTTER_A1,sr_x1_fp) + fp_mult(BUTTER_A2,sr_x2_fp) - fp_mult(BUTTER_B1,sr_y1_fp) - fp_mult(BUTTER_B2,sr_y2_fp);

        sr_x2_fp = sr_x1_fp;
        sr_x1_fp = sr_raw_fp;
        sr_y2_fp = sr_y1_fp;
        sr_y1_fp = sr_filtered_fp;
        
        // Convert back and give the values to control.c in regular format.        
        r = (int16_t)(sr_filtered_fp >> FP_SHIFT_AMOUNT);
        r = -r; // Invert yaw rate for control

    }
    else
    {        
        // Only Correct DC offset
        r = sr - sr_offset;        
        r = -r; // Invert yaw rate for control
    }
    
}


/* Function to run the Filters for Roll angle and rate.
phi and p must be set correctly at the end of this function */
void roll_filter(void)
{
    int16_t sp_corrected, say_corrected;
    int32_t sphi_raw_fp, sp_raw_fp, sp_filtered_fp; //sphi_filtered_fp

    /* Filter only when not in dmp mode */
    if(!dmp_used)
    {
        /* Get sphi from sensor values */
        say_corrected = say - say_offset;
        sphi = say_corrected;

        /* Correct rate DC offset */
        sp_corrected = sp - sp_offset;
        
        // Convert to fixed point
        sphi_raw_fp = (sphi << FP_SHIFT_AMOUNT);
        sp_raw_fp = (sp_corrected << FP_SHIFT_AMOUNT);
        // Multiply butter params
        // sphi_filtered_fp = fp_mult(BUTTER_A0,sphi_raw_fp) + fp_mult(BUTTER_A1,sphi_x1_fp) + fp_mult(BUTTER_A2,sphi_x2_fp) - fp_mult(BUTTER_B1,sphi_y1_fp) - fp_mult(BUTTER_B2,sphi_y2_fp);
        // sphi_x2_fp = sphi_x1_fp;
        // sphi_x1_fp = sphi_raw_fp;
        // sphi_y2_fp = sphi_y1_fp;
        // sphi_y1_fp = sphi_filtered_fp;

        sp_filtered_fp = fp_mult(BUTTER_A0,sp_raw_fp) + fp_mult(BUTTER_A1,sp_x1_fp) + fp_mult(BUTTER_A2,sp_x2_fp) - fp_mult(BUTTER_B1,sp_y1_fp) - fp_mult(BUTTER_B2,sp_y2_fp);
        sp_x2_fp = sp_x1_fp;
        sp_x1_fp = sp_raw_fp;
        sp_y2_fp = sp_y1_fp;
        sp_y1_fp = sp_filtered_fp;

        /* Kalman Filter */
        p_fp = sp_raw_fp - p_b_fp;
        phi_fp = phi_fp + fp_mult(p_fp,P2PHI);        
        //phi_fp = phi_fp - ((phi_fp - sphi_filtered_fp) >> ROLL_C1_SHIFT);
        phi_fp = phi_fp - ((phi_fp - sphi_raw_fp) >> ROLL_C1_SHIFT);
        p_b_fp = p_b_fp + ((phi_fp - sphi_raw_fp) >> ROLL_C2_SHIFT);

        // FOR NOW: Convert back and give the values to control.c in regular format.
        phi = (int16_t)(phi_fp >> FP_SHIFT_AMOUNT);
        p = (int16_t)(sp_filtered_fp >> FP_SHIFT_AMOUNT);
    }
    else
    {        
        /* Only correct DC offsets */
        phi = sphi - sphi_offset;
        p = sp - sp_offset;
    }

}


/* Function to run the Filters for Pitch angle and rate. 
theta and q must be set correctly at the end of this function */
void pitch_filter(void)
{
    int16_t sq_corrected, sax_corrected;
    int32_t stheta_raw_fp, sq_raw_fp, sq_filtered_fp; // stheta_filtered_fp

    /* Filter only when not in dmp mode */
    if(!dmp_used)
    {
        /* Get stheta from sensor values */
        sax_corrected = sax - sax_offset;
        stheta = sax_corrected;

        /* Correct rate DC offset */
        sq_corrected = sq - sq_offset;
        sq_corrected = -sq_corrected; // Invert pitch rate for control

        // Convert to fixed point
        stheta_raw_fp = (stheta << FP_SHIFT_AMOUNT);
        sq_raw_fp = (sq_corrected << FP_SHIFT_AMOUNT);
        // Multiply butter params
        // stheta_filtered_fp = fp_mult(BUTTER_A0,stheta_raw_fp) + fp_mult(BUTTER_A1,stheta_x1_fp) + fp_mult(BUTTER_A2,stheta_x2_fp) - fp_mult(BUTTER_B1,stheta_y1_fp) - fp_mult(BUTTER_B2,stheta_y2_fp);
        // stheta_x2_fp = stheta_x1_fp;
        // stheta_x1_fp = stheta_raw_fp;
        // stheta_y2_fp = stheta_y1_fp;
        // stheta_y1_fp = stheta_filtered_fp;

        sq_filtered_fp = fp_mult(BUTTER_A0,sq_raw_fp) + fp_mult(BUTTER_A1,sq_x1_fp) + fp_mult(BUTTER_A2,sq_x2_fp) - fp_mult(BUTTER_B1,sq_y1_fp) - fp_mult(BUTTER_B2,sq_y2_fp);
        sq_x2_fp = sq_x1_fp;
        sq_x1_fp = sq_raw_fp;
        sq_y2_fp = sq_y1_fp;
        sq_y1_fp = sq_filtered_fp;

        /* Kalman Filter */
        q_fp = sq_raw_fp - q_b_fp;
        theta_fp = theta_fp + fp_mult(q_fp,Q2THETA);
        theta_fp = theta_fp - ((theta_fp - stheta_raw_fp) >> PITCH_C1_SHIFT);
        q_b_fp = q_b_fp + ((theta_fp - stheta_raw_fp) >> PITCH_C2_SHIFT);

        // Convert back and give the values to control.c in regular format.
        theta = (int16_t)(theta_fp >> FP_SHIFT_AMOUNT);
        q = (int16_t)(sq_filtered_fp >> FP_SHIFT_AMOUNT);
    }
    else
    {
        /* Only correct DC offsets */
        theta = stheta - stheta_offset;
        q = sq - sq_offset;
        q = -q; // Invert pitch rate for control
    }     
}

void yaw_sensoracq_and_filters(void)
{
    
    // if(biggestmeasurement)
    // {
    //     stopMeasurement(STATE_MACHINE_MEAS);
    //     biggestmeasurement = 0;
    // }
    if (check_sensor_int_flag())
    {
        //biggestmeasurement = 1;
        //startMeasurement(STATE_MACHINE_MEAS); // Max 4ms in DMP and 0.8ms in RAW
        if(dmp_used)
        {
            //startMeasurement(DMP_MEAS);// 3.8ms taken!!!
            get_dmp_data();                
            //stopMeasurement(DMP_MEAS);// 3.8ms taken!!!
        }
		else
        {
            //startMeasurement(RAW_MEAS);// Just 0.65ms
            get_raw_sensor_data();             
             //stopMeasurement(RAW_MEAS);// Just 0.65ms
        }
        
        /* run filter (Only offset correction in DMP mode) */
        yaw_rate_filter();
      
	}

}

void full_sensoracq_and_filters(void)
{    
    // if(biggestmeasurement)
    // {
    //     stopMeasurement(STATE_MACHINE_MEAS);
    //     biggestmeasurement = 0;
    // }
    if (check_sensor_int_flag())
    {
        //biggestmeasurement = 1;
        //startMeasurement(STATE_MACHINE_MEAS); // Max 4ms in DMP and 0.8ms in RAW
        if(dmp_used)
        {
            //startMeasurement(DMP_MEAS);// 3.8ms taken!!!
            get_dmp_data();                
            //stopMeasurement(DMP_MEAS);// 3.8ms taken!!!
        }
		else
        {
            //startMeasurement(RAW_MEAS);// Just 0.65ms
            get_raw_sensor_data();              
             //stopMeasurement(RAW_MEAS);// Just 0.65ms
        }
        
        /* run filters (Only offset correction in DMP mode) */
        yaw_rate_filter();
	    roll_filter();
	    pitch_filter();

        // TEMP Datalogging
        // If datalog is enabled, log at maximum speed
        // if(datalog_enable){
        //     if(!datalog_write(5)){
        //         printf("Flash full; Logging complete\n");
        //         datalog_enable = false;
        //     }            
        // }   
	}
}


void hight_sensoracq_and_filters(void)    
{    
    // if(biggestmeasurement)
    // {
    //     stopMeasurement(STATE_MACHINE_MEAS);
    //     biggestmeasurement = 0;
    // }
    if (check_sensor_int_flag())
    {
        //biggestmeasurement = 1;
        //startMeasurement(STATE_MACHINE_MEAS); // Max 4ms in DMP and 0.8ms in RAW
        if(dmp_used)
        {
            //startMeasurement(DMP_MEAS);// 3.8ms taken!!!
            get_dmp_data();                
            //stopMeasurement(DMP_MEAS);// 3.8ms taken!!!
        }
		else
        {
            //startMeasurement(RAW_MEAS);// Just 0.65ms
            get_raw_sensor_data();              
             //stopMeasurement(RAW_MEAS);// Just 0.65ms
        }
        
        /* run filters (Only offset correction in DMP mode) */
        yaw_rate_filter();
	    roll_filter();
	    pitch_filter();
	}


}