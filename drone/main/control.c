/*------------------------------------------------------------------
 *  control.c -- Control algorithm and Motor mappings		 
 *		 remember! motor input =  0-1000 : 125-250 us (OneShot125)
 * 
 *  Embedded Software Lab
 *
 *  May 2019
 *
 * 	Authors: Snehal Jauhri, Ji Hang
 *------------------------------------------------------------------
 */

#include "in4073.h"
#include "control.h"
#include "calibration.h"
#include "messageDecoder.h"
#include "filters.h"
#include "profiling.h"

int16_t P_yaw = PYAW_OPTIMUM;
int16_t P1_roll = P1_OPTIMUM;
int16_t P2_roll = P2_OPTIMUM;
int16_t P1_pitch = P1_OPTIMUM;
int16_t P2_pitch = P2_OPTIMUM;
int16_t PH_hight = PH_OPTIMUM;
int16_t lift_setpoint;
int32_t baro_setpoint;

void update_motors(void)
{	
    for(int i=0; i<4; i++)
	{
		if (ae[i] > MAX_SPEED)
		{
			motor[i] = MAX_SPEED;
		}
		else if (ae[i] < MIN_SPEED)
		{			
			motor[i] = 0; // Setting to zero if below minimum
		}
		else 
		{
			motor[i] = ae[i];
		}
	}	
}


void read_baro_setpoint(){
	lift_setpoint = lift;
	read_baro();
	baro_setpoint = pressure;
}


void shutoff_motors(void)
{	
    for(int i=0; i<4; i++)
	{
		ae[i] = 0;
		motor[i] = 0;
	}
}

/*------------------------------------------------------------------
 * run_full_control -- use inputs and run in full control mode
 *------------------------------------------------------------------
 */
void run_full_control(int16_t roll, int16_t pitch, int16_t yaw, int16_t lift)
{
	//startMeasurement(FULLCTRL_MEAS);

	/* map lift values first */
	int16_t lift_ret;
	lift_ret = map_lift(lift);

	/* First check if there is lift */
	if(lift_ret >= MIN_SPEED)
	{
		/* map roll, pitch and yaw */
		map_roll_control(roll, P1_roll, P2_roll);
		map_pitch_control(pitch, P1_pitch, P2_pitch);
		map_yaw_control(yaw, P_yaw);

		/* Ensure min speed */
		for(int i=0; i<4; i++)
		{
			if (ae[i] < MIN_SPEED)
			{
				ae[i] = MIN_SPEED;
			}
		}
	}
	else
	{
		/* Not enough lift, set all ae to zero */
		for(int j=0; j<4; j++)
		{
			ae[j] = 0;
		}

	}

	/* Finally, update the ae values to motors */
	update_motors();
	//stopMeasurement(FULLCTRL_MEAS); // Max 0.1ms
}

/*------------------------------------------------------------------
 * run_hight_control -- use inputs and run in higth control mode
 *------------------------------------------------------------------
 */
void run_hight_control(int16_t roll, int16_t pitch, int16_t yaw)
{
	//startMeasurement(FULLCTRL_MEAS);	

	/* map lift values first */
	int16_t lift_ret = map_lift(lift_setpoint);
	map_lift_hight();
	/* First check if there is lift */
	if(lift_ret >= MIN_SPEED)
	{
		/* map roll, pitch and yaw */
		map_roll_control(roll, P1_roll, P2_roll);
		//map_roll(roll);
		map_pitch_control(pitch, P1_pitch, P2_pitch);
		//map_pitch(pitch);		
		map_yaw_control(yaw, P_yaw);
		//map_yaw(yaw);

		/* Ensure min speed */
		for(int i=0; i<4; i++)
		{
			if (ae[i] < MIN_SPEED)
			{
				ae[i] = MIN_SPEED;
			}
		}
	}
	else
	{
		/* Not enough lift, set all ae to zero */
		for(int j=0; j<4; j++)
		{
			ae[j] = 0;
		}

	}

	/* Finally, update the ae values to motors */
	update_motors();
	//stopMeasurement(FULLCTRL_MEAS); // Max 0.1ms
}

/*------------------------------------------------------------------
 * run_yaw_control -- use inputs and run in yaw control mode
 *------------------------------------------------------------------
 */
void run_yaw_control(int16_t roll, int16_t pitch, int16_t yaw, int16_t lift)
{

	/* map lift values first */
	int16_t lift_ret;
	lift_ret = map_lift(lift);

	/* First check if there is lift */
	if(lift_ret >= MIN_SPEED)
	{
		/* map roll, pitch and yaw */
		map_roll(roll);
		map_pitch(pitch);
		map_yaw_control(yaw, P_yaw);

		/* Ensure min speed */
		for(int i=0; i<4; i++)
		{
			if (ae[i] < MIN_SPEED)
			{
				ae[i] = MIN_SPEED;
			}
		}
	}
	else
	{
		/* Not enough lift, set all ae to zero */
		for(int j=0; j<4; j++)
		{
			ae[j] = 0;
		}

	}

	/* Finally, update the ae values to motors */
	update_motors();

}

/*------------------------------------------------------------------
 * update_ae -- Linear Mapping of inputs to rotor rpms
 *------------------------------------------------------------------
 */
void update_ae(int16_t roll, int16_t pitch, int16_t yaw, int16_t lift)
{
	/* First check if there is lift */
	int16_t lift_ret;
	lift_ret = map_lift(lift);	

	/* First check if there is lift */
	if(lift_ret >= MIN_SPEED)
	{
		/* map roll, pitch and yaw */
		map_roll(roll);
		map_pitch(pitch);
		map_yaw(yaw);

		/* Ensure min speed */
		for(int i=0; i<4; i++)
		{
			if (ae[i] < MIN_SPEED)
			{
				ae[i] = MIN_SPEED;
			}
		}
	}
	else
	{
		/* Not enough lift, set all ae to zero */
		for(int j=0; j<4; j++)
		{
			ae[j] = 0;
		}

	}	

}

// joystick_map_lift: Maps to between 0 to 1024
int16_t joystick_map_lift(int16_t val)
{
	int32_t newVal = val + INT16_MAX + 1;		// +1 bcs negative range is 1 bigger than positive in int16_t
	newVal = newVal >> 6;	

	return newVal;
}

//map lift
int16_t map_lift(int16_t val_lift)
{	
	//achieve lift by changing the rpm of ae[3] and ae[1] & ae[0] and ae[2]
	int16_t lift_s;
	lift_s = joystick_map_lift(val_lift);
	// printf("Joystick mapped lift: %d\n", lift);
	ae[0] = lift_s;
	ae[1] = lift_s;
	ae[2] = lift_s;
	ae[3] = lift_s;
	
	return lift_s;
}

//map lift for hight control using pressure
void map_lift_hight(void)
{	
	int16_t difference = (PH_hight * (int16_t)(baro_setpoint - pressure)) >> 3;
	
//	int16_t diff = difference >> 13;
	// printf("Joystick mapped lift: %d\n", lift);
	ae[0] -= difference;
	ae[1] -= difference;
	ae[2] -= difference;
	ae[3] -= difference;
	// printf("lift: ae[0]=%d, ae[1]=%d, ae[2]=%d, ae[3]=%d\n",ae[0],ae[1],ae[2],ae[3]);
}

/*------------------------------------------------------------------
 *  mapping function for roll, pitch
 *  change -32767~32767 to 0~128
 *------------------------------------------------------------------
 */
int16_t joystick_map(int16_t val)
{

	if ( val > 0 )
	{
		val = val>>R_SHIFT;
		return val;
	}
	else
	{
		val = ~val+1;
		val = val>>R_SHIFT;
		return val;
	}

}

/*------------------------------------------------------------------
 *  mapping function for yaw
 *  change -32767~32767 to 0~255
 *------------------------------------------------------------------
 */
int16_t joystick_map_yaw(int16_t val)
{

	if ( val > 0 )
	{
		val = val>>R_SHIFT_YAW;
		return val;
	}
	else
	{
		val = ~val+1;
		val = val>>R_SHIFT_YAW;
		return val;
	}

}


void map_roll(int16_t val_roll)
{	
	//achieve roll by the rpm difference of ae[3] and ae[1]
	uint16_t roll;
	roll = joystick_map(val_roll);

	if (val_roll > 0)
	{
		ae[1] = ae[1] - roll;
		ae[3] = ae[3] + roll;
	}
	else
	{
		//other direction  
		ae[1] = ae[1] + roll;
		ae[3] = ae[3] - roll;
	}	
			
}


void map_pitch(int16_t val_pitch)
{		
	//achieve pitch by the rpm difference of ae[0] and ae[2] 
	uint16_t pitch;
	pitch = joystick_map(val_pitch); 

	if(val_pitch > 0)
	{
		ae[0] = ae[0] + pitch;
		ae[2] = ae[2] - pitch;
	}
	else
	{
		//other direction 
		ae[0] = ae[0] - pitch;
		ae[2] = ae[2] + pitch;
	}
		

}


void map_yaw(int16_t val_yaw)
{	
	
	/* achieve yaw by the rpm difference of ae[3] and ae[1] & ae[0] and ae[2] */
	uint16_t yaw;
	
	yaw = joystick_map_yaw(val_yaw);

	if ( val_yaw > 0 )
	{
		ae[3] = ae[3] + yaw;
		ae[1] = ae[1] + yaw;

		ae[2] = ae[2] - yaw;
		ae[0] = ae[0] - yaw;
	}
	else
	{
		//other direction
		ae[3] = ae[3] - yaw;
		ae[1] = ae[1] - yaw;

		ae[2] = ae[2] + yaw;
		ae[0] = ae[0] + yaw;   

	}	

}


void map_roll_control(int16_t val_roll, int16_t val_P1_roll, int16_t val_P2_roll)
{
	//achieve roll by the rpm difference of ae[3] and ae[1]
	uint16_t roll;
	int16_t p_s;
	int16_t phi_s;
	int32_t roll_ctrl;
	int16_t roll_ctrl_bounded;

	p_s = p >> SENS_RATE_DIVIDER;
	phi_s = phi >> SENS_ANGLE_DIVIDER;	

	/* phi -16384~16384 -> -180 degree ~ +180 degree */	
	val_roll = val_roll >> SETPOINT_DIVIDER_ROLL; // right shift 3 bits, divide by 8, scale down to -45 degree to 45 degree
	
	/*Cascade control of roll, val_P1_roll, val_P2_roll: control parameters*/	
	roll_ctrl = val_P1_roll*(val_roll - phi_s) - val_P2_roll*p_s;	
	
	/* Bound the value of roll_ctrl */
	if(roll_ctrl > INT16_MAX)
	{
		roll_ctrl_bounded = INT16_MAX;
	}
	else if (roll_ctrl < -INT16_MAX)
	{
		roll_ctrl_bounded = -INT16_MAX;
	}
	else
	{
		roll_ctrl_bounded = (int16_t)roll_ctrl;
	}

	roll = joystick_map(roll_ctrl_bounded);

	if (roll_ctrl_bounded > 0)
	{
		ae[1] = ae[1] - roll;
		ae[3] = ae[3] + roll;
	}
	else
	{
		ae[1] = ae[1] + roll;
		ae[3] = ae[3] - roll;
	}	

}


void map_pitch_control(int16_t val_pitch, int16_t val_P1_pitch, int16_t val_P2_pitch)
{
	//achieve pitch by the rpm difference of ae[0] and ae[2]
	uint16_t pitch;
	int16_t q_s;
	int16_t theta_s;
	int32_t pitch_ctrl;
	int16_t pitch_ctrl_bounded;
	
	q_s = q >> SENS_RATE_DIVIDER;;	
	theta_s = theta >> SENS_ANGLE_DIVIDER;;		
	
	/* theta -16384~16384 -> -180 degree ~ +180 degree */	
	val_pitch = val_pitch >> SETPOINT_DIVIDER_PITCH; // right shift 3 bits, divide by 8, scale down to -45 degree to 45 degree

	/*Cascade control of pitch, val_P1_pitch, val_P2_pitch: control parameters*/	
	pitch_ctrl = val_P1_pitch*(val_pitch - theta_s) - val_P2_pitch*q_s;

	/* Bound the value of pitch_ctrl */
	if(pitch_ctrl > INT16_MAX)
	{
		pitch_ctrl_bounded = INT16_MAX;
	}	
	else if (pitch_ctrl < -INT16_MAX)
	{
		pitch_ctrl_bounded = -INT16_MAX;
	}
	else
	{
		pitch_ctrl_bounded = (int16_t)pitch_ctrl;
	}

	pitch = joystick_map(pitch_ctrl_bounded);

	if (pitch_ctrl_bounded > 0)
	{
		ae[0] = ae[0] + pitch;
		ae[2] = ae[2] - pitch;
	}
	else
	{
		//other direction 
		ae[0] = ae[0] - pitch;
		ae[2] = ae[2] + pitch;
	}

}


void map_yaw_control(int16_t val_yaw, int16_t val_P)
{	
	
	//achieve yaw by the rpm difference of ae[3] and ae[1] & ae[0] and ae[2]
	uint16_t yaw;
	int16_t r_s, yaw_ctrl_bounded;
	int32_t yaw_ctrl;

	r_s = r;

	val_yaw = val_yaw >> SETPOINT_DIVIDER_YAW; // Divide setpoint

	/* Control law. P should be in the range 1 to 10ish */
	yaw_ctrl = (val_P)*(val_yaw-r_s);

	/* Bound the value of yaw_ctrl */
	if(yaw_ctrl > INT16_MAX)
	{
		yaw_ctrl_bounded = INT16_MAX;
	}	
	else if (yaw_ctrl < -INT16_MAX)
	{
		yaw_ctrl_bounded = -INT16_MAX;
	}
	else
	{
		yaw_ctrl_bounded = (int16_t)yaw_ctrl;
	}
	
	yaw = joystick_map_yaw(yaw_ctrl_bounded);	

	if (yaw_ctrl_bounded > 0)
	{
		ae[3] = ae[3] + yaw;
		ae[1] = ae[1] + yaw;

		ae[2] = ae[2] - yaw;
		ae[0] = ae[0] - yaw;

	}
	else
	{
		//another direction 
		ae[3] = ae[3] - yaw;
		ae[1] = ae[1] - yaw;

		ae[2] = ae[2] + yaw;
		ae[0] = ae[0] + yaw;   

	}		

}