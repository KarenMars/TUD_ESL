 /*
 ============================================================================
 Name        : profiling.h
 Author      : Jelger
 Version     :
 Copyright   :
 Description : Measures the time spend in various loops/functions
 ============================================================================
 */

#ifndef PROFILING_H
#define PROFILING_H

#include <inttypes.h>
#include <stdio.h>

#define MAX_MEAS           10
#define STATE_MACHINE_MEAS  0
#define FULLCTRL_MEAS       1
#define FILTER_MEAS         2
#define DMP_MEAS            3
#define RAW_MEAS            4

uint32_t startTimes[MAX_MEAS];
uint32_t finishTimes[MAX_MEAS];
uint32_t execTimes[MAX_MEAS];
uint16_t numexecutions[MAX_MEAS];

void initProfiling(void);
void startMeasurement(uint8_t number);
void stopMeasurement(uint8_t number);
void executed(uint8_t number);

#endif      // PROFILING