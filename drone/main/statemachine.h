/*------------------------------------------------------------------
 *  statemachine.h -- defines, globals, function prototypes for the overall state machine
 *
 *  I. Snehal Jauhri
 *  Embedded Software Lab
 *
 *  May 2019
 *------------------------------------------------------------------
 */

#ifndef STATEMACHINE_H__
#define STATEMACHINE_H__

/* Flight Messages safety check */
#define MIN_FLIGHT_COUNT  5
#define ZERO_FLIGHT_COUNT 0
#define FIFTY_MS          500000

extern enum modes current_mode;

enum modes
{
    safe,
    panic,
    manual,
    calib,
    yaw_ctrl,
    full_ctrl,
    use_raw, 
    use_dmp,
    height,
    sendinglog
};

void batteryCheck();
bool drone_fsm(void);

#endif // STATEMACHINE_H__