 /*
 ============================================================================
 Name        : profiling.c
 Author      : Jelger
 Version     :
 Copyright   :
 Description : Measures the time spend in various loops/functions
 ============================================================================
 */

#include "profiling.h"
#include "in4073.h"
#include <inttypes.h>
#include <stdio.h>

void initProfiling()
{
	for (uint8_t i = 0; i < MAX_MEAS; i++) {
		startTimes[i] = 0;
		finishTimes[i] = 0;
		execTimes[i] = 0;
		numexecutions[i] = 0;
	}
}

void startMeasurement(uint8_t number)
{
	startTimes[number] = get_time_us();
}

void stopMeasurement(uint8_t number)
{
	finishTimes[number] =  get_time_us();
	execTimes[number] = finishTimes[number] - startTimes[number];
}

void executed(uint8_t number)
{
	numexecutions[number]++;
}

