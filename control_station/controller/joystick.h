#ifndef JOYSTICK_H
#define JOYSTICK_H
/*
    joystick.h
    Joystick stuff header file
    Author: Jure Vidmar
*/
#include "joystick_driver.h"

#define JOYSTICK_PATH       "/dev/input/js0"
#define NAME_LENGTH         128

/* Joystick event types */
#define JS_BTN_EVENT            1
#define JS_AXIS_EVENT           2

/* Joystcik buttons */
#define JS_PANIC_BTN            0

/* Joystick axes */
#define JS_ROLL_AXIS            0
#define JS_PITCH_AXIS           1
#define JS_YAW_AXIS             2
#define JS_LIFT_AXIS            3

// Global joystick state
int *axis;
char *button;
int i;
struct js_event js;


int init_js();
void process_js();


#endif      /* JOYSTICK_H */