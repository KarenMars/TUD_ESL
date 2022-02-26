/*
-----------------------------------------------------------------
19-05-2019 calibration.h

Author: Ji Hang
-----------------------------------------------------------------
*/
#ifndef CALIBRATION_H__
#define CALIBRATION_H__

#include "app_util_platform.h"

#define SAMPLE 512
#define DIV 9

int16_t data_cnt;
int16_t sp_offset;
int16_t sq_offset;
int16_t sr_offset;
int16_t sax_offset;
int16_t say_offset;
int16_t saz_offset;

int16_t sphi_offset;
int16_t stheta_offset;
int16_t spsi_offset;

void static_calibration(void);

#endif // CALIBRATION_H