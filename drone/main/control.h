/*------------------------------------------------------------------
 *  control.h -- Definitions for Control algorithm and Motor mappings		 
 *		 remember! motor input =  0-1000 : 125-250 us (OneShot125)
 * 
 *  Embedded Software Lab
 *
 *  May 2019
 *
 * 	Authors: Snehal Jauhri, Ji Hang
 *------------------------------------------------------------------
 */

#ifndef CONTROL_H__
#define CONTROL_H__

#define MAX_SPEED 850
#define MIN_SPEED 200
#define R_SHIFT   8 
#define R_SHIFT_YAW 7

/* Scaling for Moments */
#define LIFT_SCALER    30
#define ROT_SCALER     15
#define MIN_Z          40000
#define LIFT_REDUCTION 1280

#define SENS_RATE_DIVIDER 1
#define SENS_ANGLE_DIVIDER 1
#define SETPOINT_DIVIDER_YAW  4
#define SETPOINT_DIVIDER_ROLL 3
#define SETPOINT_DIVIDER_PITCH 3

#define PYAW_OPTIMUM        24
#define P1_OPTIMUM          17
#define P2_OPTIMUM          46
#define PH_OPTIMUM          60

/* Global control parameters */
extern int16_t P_yaw;
extern int16_t P1_roll;
extern int16_t P2_roll;
extern int16_t P1_pitch;
extern int16_t P2_pitch;
extern int16_t PH_hight;
extern int16_t lift_setpoint;

int16_t joystick_map(int16_t val);
int16_t joystick_map_yaw(int16_t val);
int16_t joystick_map_lift(int16_t val);
void update_ae(int16_t roll, int16_t pitch, int16_t yaw, int16_t lift);

void read_baro_setpoint();
void update_motors(void);
void shutoff_motors(void);

void run_yaw_control(int16_t roll, int16_t pitch, int16_t yaw, int16_t lift);
void run_full_control(int16_t roll, int16_t pitch, int16_t yaw, int16_t lift);
void run_hight_control(int16_t roll, int16_t pitch, int16_t yaw);

void map_roll(int16_t val_roll);
void map_pitch(int16_t val_pitch);
void map_yaw(int16_t val_yaw);
void map_lift_hight(void);

void map_roll_control(int16_t val_roll, int16_t val_P1_roll, int16_t val_P2_roll);
void map_pitch_control(int16_t val_pitch, int16_t val_P2_roll, int16_t val_P2_pitch);
void map_yaw_control(int16_t val_yaw, int16_t val_P);
int16_t map_lift(int16_t val_lift);

#endif // CONTROL_H__