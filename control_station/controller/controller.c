/*
    controller.c
    Control station -> program handling the communication between joystick and drone
	Author: Jure Vidmar
*/
#include "terminal.h"
#include "joystick.h"
#include "controller.h"
#include "packet.h"
#include "queue.h"
#include "pkt_receiver.h"

PacketReceiver pkt_rx;
queue tx_queue;
queue rx_queue;

// TEMP GUI STUFF
FILE *guifile;


/* Global control parameters */
int16_t P_yawc = PYAW_OPTIMUM;
int16_t P1_rollc = P1_OPTIMUM;
int16_t P2_rollc = P2_OPTIMUM;
int16_t P1_pitchc = P1_OPTIMUM;
int16_t P2_pitchc = P2_OPTIMUM;
int16_t PH_hightc = PH_OPTIMUM;

void process_keys (char c)
{
	switch (c) {

		// Arrow key pressed
        case '\033':
			getchar();     		// Skip the meta-character
            c = getchar();

            switch (c) {

				// Arrow up
				case 'A':
					trim_fp(&key_params.pitch, +FP_STEP);
					update_fp();
					printf("Pitch up -> trim: %d\tpitch: %d\n", key_params.pitch, fp.pitch);
					break;
				
				// Arrow down
				case 'B':
					trim_fp(&key_params.pitch, -FP_STEP);
					update_fp();
					printf("Pitch down -> trim: %d\tpitch: %d\n", key_params.pitch, fp.pitch);
					break;

				// Arrow right
				case 'C':
					trim_fp(&key_params.roll, -FP_STEP);
					update_fp();
					printf("Roll down -> trim: %d\troll: %d\n", key_params.roll, fp.roll);
					break;

				// Arrow left
				case 'D':
					trim_fp(&key_params.roll, +FP_STEP);
					update_fp();
					printf("Roll up -> trim: %d\troll: %d\n", key_params.roll, fp.roll);
					break;
				
				default:
					// printf("Unkown arrow pressed!\n");
					printf("Emergency!!!\n");
					change_mode(MODE_PANIC);
					break;

			}
			break;

		case 'a':
			trim_fp(&key_params.lift, +FP_STEP);
			update_fp();
			printf("Lift up -> trim: %d\tlift: %d\n", key_params.lift, fp.lift);
			break;

		case 'z':
		case 'y':
			trim_fp(&key_params.lift, -FP_STEP);
			update_fp();
			printf("Lift down -> trim: %d\tlift: %d\n", key_params.lift, fp.lift);
			break; 

		case 'q':
			trim_fp(&key_params.yaw, +FP_STEP);
			update_fp();
			printf("Yaw up -> trim: %d\t yaw:%d\n", key_params.yaw, fp.yaw);
			break;

		case 'w':
			trim_fp(&key_params.yaw, -FP_STEP);
			update_fp();
			printf("Yaw down -> trim: %d\t yaw:%d\n", key_params.yaw, fp.yaw);
			break;
		
		case 'u':			
			P_yawc += 1;
			printf("P @yaw control -> up = %d,\n",P_yawc);
			create_ctrl_pkt(&tx_queue, P_yawc, P1_rollc, P2_rollc, PH_hightc);
			break;
		
		case 'j':
			P_yawc -= 1;
			if(P_yawc < 0)
			{
				P_yawc = 0;
			}
			printf("P @yaw control -> down = %d,\n",P_yawc);
			create_ctrl_pkt(&tx_queue, P_yawc, P1_rollc, P2_rollc, PH_hightc);
			break;

		case 'i':			
			P1_rollc += 1;
			P1_pitchc += 1;
			printf("P1 @roll/pitch -> up = %d,\n",P1_rollc);			
			create_ctrl_pkt(&tx_queue, P_yawc, P1_rollc, P2_rollc, PH_hightc);
			break;

		case 'k':			
			P1_rollc -= 1;
			P1_pitchc -= 1;
			if(P1_rollc < 0)
			{
				P1_rollc = 0;
				P1_pitchc = 0;
			}
			printf("P1 @roll/pitch -> down = %d,\n",P1_rollc);			
			create_ctrl_pkt(&tx_queue, P_yawc, P1_rollc, P2_rollc, PH_hightc);
			break;

		case 'o':
			P2_rollc += 1;
			P2_pitchc += 1;
			printf("P2 @roll/pitch -> up = %d,\n",P2_rollc);			
			create_ctrl_pkt(&tx_queue, P_yawc, P1_rollc, P2_rollc, PH_hightc);				
			break;

		case 'l':
			P2_rollc -= 1;
			P2_pitchc -= 1;
			if(P2_rollc < 0)
			{
				P2_rollc = 0;
				P2_pitchc = 0;
			}
			printf("P2 @roll/pitch -> down = %d,\n",P2_rollc);			
			create_ctrl_pkt(&tx_queue, P_yawc, P1_rollc, P2_rollc, PH_hightc);				
			break;
		case 'p':
			PH_hightc += 1;
			printf("PH @hight -> up = %d,\n",PH_hightc);			
			create_ctrl_pkt(&tx_queue, P_yawc, P1_rollc, P2_rollc, PH_hightc);				
			break;
		case ';':
			PH_hightc -= 1;
			if(PH_hightc < 0)
			{
				PH_hightc = 0;
			}
			printf("PH @hight -> down = %d,\n",PH_hightc);			
			create_ctrl_pkt(&tx_queue, P_yawc, P1_rollc, P2_rollc, PH_hightc);				
			break;
		case '[':
			printf("Datalog start!\n");
			create_log_start_pkt(&tx_queue);
			break;

		case ']':
			printf("Datalog data request!\n");
			create_log_request_pkt(&tx_queue);
			break;
		/*----------------------------------------*/


		/* MODE SWITCHING */
		case '0':
			printf("Safe mode!\n");
			change_mode(MODE_SAFE);
			break;
		
		case '1':
			printf("Emergency!!!\n");
			change_mode(MODE_PANIC);
			break;

		case '2':
			printf("Manual mode!\n");
			change_mode(MODE_MANUAL);
			break;

		case '3':
			printf("Calibration mode!\n");
			change_mode(MODE_CALIBRATION);
			break;

		case '4':
			printf("Yaw controlled mode!\n");
			change_mode(MODE_YAW_CTRL);
			break;

		case '5':
			printf("Full control mode!\n");
			change_mode(MODE_FULL_CTRL);
			break;

		case '6':
			printf("Raw mode!\n");
			change_mode(MODE_RAW);
			break;
		
		case '7':
			printf("DMP mode!\n");
			change_mode(MODE_DMP);
			break;			
		
		case '8':
			printf("Height control mode!\n");
			change_mode(MODE_HEIGHT_CTRL);
			break;

		case '9':
			printf("Wireless mode!\n");
			change_mode(MODE_WIRELESS);
			break;

		case 'x':
			fclose(guifile);
			rs232_close();
			term_exitio();
			exit(0);
			break;

		default:
			printf("Unknown btn: %d\n", c);
			break;
	}
}


/*
	Wrapper for checking if mode_switch is allowed + packet creation
*/
void change_mode(uint8_t mode) 
{
    // Check if it's safe to change mode
    if (canI_changeMode(mode) == 0) {
		create_mode_pkt(&tx_queue, mode);
    } else {
        printf("Joystick not in initial position, not switching mode...!\n");
        return;
	}
}

/*
	Helper function for checking if it is safe to switch modes
	Returns 0 if OK, -1 otherwise
*/
int8_t canI_changeMode(uint8_t mode)
{
    // No checking needed when switching to panic or safe mode (should always be possible)
    if ((mode == MODE_PANIC) || (mode == MODE_SAFE) || (mode == MODE_CALIBRATION) || (mode == MODE_RAW) || (mode == MODE_DMP) || (mode == MODE_HEIGHT_CTRL)) {
        return 0;
    }

    // Check joystick/fp position
    #define ZERO_POS_THRESH       300
    if ((fp.roll > ZERO_POS_THRESH)  || (fp.roll < -ZERO_POS_THRESH)) return -1;
    if ((fp.pitch > ZERO_POS_THRESH) || (fp.pitch < -ZERO_POS_THRESH)) return -1;
    if ((fp.yaw > ZERO_POS_THRESH)   || (fp.yaw < -ZERO_POS_THRESH)) return -1;
    if (fp.lift > FP_MIN_VAL+ZERO_POS_THRESH) return -1;
    
    // Everything is at zero -> safe to change mode
    return 0;

}

/*
    Receive bytes from serial port
    @max_bytes -> maximum number of bytes to be received in one loop iteration
*/
void serial_getbytes (queue *q, uint16_t max_bytes)
{
    uint16_t cnt = 0;        // counter of received bytes
    int16_t c;               // Received byte

    while (((c = rs232_getchar_nb()) != -1) && (cnt < max_bytes)) {

        enqueue(q, c);
        cnt++;
    }

}

/*
	Send bytes to the serial port
	@max_bytes -> maximum number of bytes to be sent in one loop iteration
*/
void serial_sendbytes (queue *q, uint16_t max_bytes)
{
	uint16_t cnt = 0;
	while ((q->count > 0) && (cnt < max_bytes)) {
		rs232_putchar(dequeue(q));
		cnt++;
	}

}


void init_joy_params()
{
	if (joystick_available) {
		// TODO: Read joystick position -> init with that
		joy_params.roll = 0;
		joy_params.pitch = 0;
		joy_params.yaw = 0;
		joy_params.lift = FP_MIN_VAL;
	} else {
		joy_params.roll = 0;
		joy_params.pitch = 0;
		joy_params.yaw = 0;
		joy_params.lift = FP_MIN_VAL;
	}

	printf("Joystick params initialized to: \t%d\t%d\t%d\t%d\n", joy_params.roll, joy_params.pitch, joy_params.yaw, joy_params.lift);
}

void init_key_params()
{
	key_params.roll = 0;
	key_params.pitch = 0;
	key_params.yaw = 0;
	key_params.lift = 0;
}

void update_fp()
{
	fp.roll = bounded_sum(joy_params.roll, key_params.roll, FP_MIN_VAL, FP_MAX_VAL);
	fp.pitch = bounded_sum(joy_params.pitch, key_params.pitch, FP_MIN_VAL, FP_MAX_VAL);
	fp.yaw = bounded_sum(joy_params.yaw, key_params.yaw, FP_MIN_VAL, FP_MAX_VAL);
	fp.lift = bounded_sum(joy_params.lift, key_params.lift, FP_MIN_VAL, FP_MAX_VAL);
}


/*
    Sum two 16-bit values, with custom boundary checking
*/
int16_t bounded_sum(int16_t a, int16_t b, int16_t min, int16_t max)
{
    
    int32_t newVal = (int32_t)a + (int32_t)b;

    if (newVal > max) {
        return max;
    } else if (newVal < min) {
        return min;
    }

    return newVal;
}


/*
	Trim flight parameters, checking the limits of each parameter
	@param 	-> pointer to the flight parameter being changed
	@value	-> value to be added to given flight parameter
*/
void trim_fp (int16_t *param, int16_t value) {

	int16_t newVal = bounded_sum(*param, value, FP_MIN_VAL, FP_MAX_VAL);
	*param = newVal;
}


int main (int argc, char **argv)
{
	printf("\n\n");
	printf("Initializing control station...\n");

	// Initialize queues
	init_queue(&rx_queue); 			// Initialize receive queue
	init_queue(&tx_queue); 			// Initialize transmit queue

	// Initialize joystick
	if (init_js() < 0) {
		joystick_available = false;
		printf("Joystick not available!\n");
	} else {
		joystick_available = true;
		printf("Joystick available\n");
	}

	// Initialize terminal & MCU serial connection
	term_initio();
	rs232_open();

	// Initialize flight params
	init_joy_params();
	init_key_params();
	update_fp();

	// Initialize packet receiver
	init_pkt_receiver(&pkt_rx, &rx_queue);
	init_log(log_buffer);

	// Initialize crude timer
	time_t start = clock();
	time_t start2 = clock();

	int c;
	while (1) {

		// OUTPUT processing
		// Check keys
		if ((c = term_getchar_nb()) != -1) {
			process_keys(c);
		}

		// Check joystick
		if (joystick_available) {
			process_js();
		}

		// Send data from tx queue
		serial_sendbytes(&tx_queue, TX_BYTES_PER_LOOP);

		// Add incoming data to RX queue
		serial_getbytes(&rx_queue, RX_BYTES_PER_LOOP);

		// Execute one step of packet receiver FSM
		(*pkt_rx.state)(&pkt_rx);

		// Run GUI @40 Hz
		if ((clock() - start2) >= CLOCKS_PER_SEC/40){		
			guifile = fopen("guifile.out", "w");			
			fprintf(guifile, "%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\n", dronemode, fp.roll, fp.pitch, fp.yaw, fp.lift, ae0, ae1, ae2, ae3, battery);			
			fclose(guifile);			
			start2 = clock();
		}

		// Send flight packet @50 Hz
		if ((clock() - start) >= CLOCKS_PER_SEC/50) {				
			create_flight_pkt(&tx_queue, fp.roll, fp.pitch, fp.yaw, fp.lift);
			start = clock();
		}

	}

	term_exitio();
	rs232_close();
	term_puts("\n<exit>\n");

	return 0;
}

