/*------------------------------------------------------------------
 *  in4073.c -- test QR engines and sensors
 *
 *  reads ae[0-3] uart rx queue
 *  (q,w,e,r increment, a,s,d,f decrement)
 *
 *  prints timestamp, ae[0-3], sensors to uart tx queue
 *
 *  I. Protonotarios
 *  Embedded Software Lab
 *
 *  June 2016
 *------------------------------------------------------------------
 */
#include "in4073.h"
#include "statemachine.h"
#include "packet.h"
#include "pkt_receiver.h"

// DEBUG
#include "calibration.h"
#include "filters.h"
#include "profiling.h"


/*
	Send up to @max_bytes bytes to the serial line
	@q 		   -> queue from which bytes should be sent
	@max_bytes -> max number of bytes to be sent at one loop
*/
void serial_sendbytes (queue *q, uint16_t max_bytes)
{

	uint16_t cnt = 0;
	while ((q->count > 0) && (cnt < max_bytes)) {
		uart_put(dequeue(q));
		cnt++;
	}
}


/*------------------------------------------------------------------
 * main -- everything you need is here :)
 *------------------------------------------------------------------
 */
int main(void)
{
	uart_init();
	gpio_init();
	timers_init();
	adc_init();
	twi_init();	
	dmp_used = true;
	imu_init(true, IMU_DMP_FREQ);
	baro_init();
	spi_flash_init();
	initProfiling();	
	adc_request_sample(); /* Initial Battery voltage */

	uint32_t counter = 0;	

	do
	{
		serial_sendbytes(&tx_queue, TX_BYTES_PER_LOOP);

		if (check_timer_flag()) 
		{
			read_baro();
			if (counter++ >= 100)
			{
				counter = 0;
				nrf_gpio_pin_toggle(BLUE); // Keeping this functionality to indicate system running
				batteryCheck();
				// DEBUG prints
    			// printf("sp: %d\n",sp-sp_offset);
    			// printf("sq: %d\n",sq-sq_offset);
				// printf("sr: %d\n",sq-sq_offset);						
    			// printf("sax: %d\n",sax-sax_offset);
    			// printf("say: %d\n",say-say_offset);
    			// printf("saz: %d\n",saz-saz_offset);
				// printf("sp: %d\n",sp);
    			// printf("sq: %d\n",sq);
				// printf("sr: %d\n",sq);						
    			// printf("sax: %d\n",sax);
    			// printf("say: %d\n",say);
    			// printf("saz: %d\n",saz);
    			// printf("phi: %d\n",phi);
    			// printf("theta: %d\n",theta);
    			// printf("psi: %d\n",psi);
				// printf("p: %d\n",p);
    			// printf("q: %d\n",q);
    			// printf("r: %d\n",r);
				//printf("Flight data: \tRoll: %d \t Pitch: %d \tYaw: %d \tLift: %d\n", roll, pitch, yaw, lift);
				//printf("State M/C Exec time: %ld\n",execTimes[STATE_MACHINE_MEAS]);
				/* log data for heartbeat and GUI */
				create_log_status_pkt(&tx_queue, current_mode, ae[0], ae[1], ae[2], ae[3], bat_volt);					
			}

			clear_timer_flag();			
		}
		
	}while (drone_fsm());

	printf("\n\t Goodbye \n\n");
	nrf_delay_ms(100);

	NVIC_SystemReset();
}
