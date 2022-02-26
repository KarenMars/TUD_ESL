/*
    controller.h
    Header for the controller
    Authors: Jure Vidmar
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "queue.h"

/* Amount of bytes to be sent/received (serial) in one loop iteration */
#define RX_BYTES_PER_LOOP          60
#define TX_BYTES_PER_LOOP          45

/* Trimming settings */
#define FP_MIN_VAL          -32767      // should be -32768 right?
#define FP_MAX_VAL           32767
#define FP_STEP               2000

// Optimum P_yaw to send
#define PYAW_OPTIMUM        24 // 11
#define P1_OPTIMUM          17 // 10
#define P2_OPTIMUM          46 // 20
#define PH_OPTIMUM          60

// Structure definition
typedef struct FlightParams FlightParams;
struct FlightParams {
    
    int16_t roll;
    int16_t pitch;
    int16_t yaw;
    int16_t lift;
};

// Global FP structures
FlightParams key_params;        // Keyboard (trim) parameters
FlightParams joy_params;        // Joystick parameters
FlightParams fp;                // Flight parameters (key + joy combined)

// Global buffer for storing logdata
uint8_t log_buffer[131072];     // 128 kiB


void init_key_params();
void init_joy_params();
void trim_fp (int16_t *param, int16_t value);
void update_fp();               // Sum keyboard and joystick parameters with boundary checking
void init_GUIcomm();

// Joystick availability flag
bool joystick_available;

// Keyboard processing func
void process_keys(char c);

int16_t bounded_sum(int16_t a, int16_t b, int16_t min, int16_t max);
void change_mode(uint8_t mode);
int8_t canI_changeMode(uint8_t mode);
void serial_getbytes (queue *q, uint16_t max_bytes);

