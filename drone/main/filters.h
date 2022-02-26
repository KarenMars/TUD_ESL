/*------------------------------------------------------------------
 *  filters.h -- Definitions for Butterworth and Kalman filters
 * 
 *  Embedded Software Lab
 *
 *  June 2019
 *
 * 	Authors: Snehal Jauhri
 *------------------------------------------------------------------
 */

#ifndef FILTERS_H__
#define FILTERS_H__

#define ANGLE_SCALER    0
#define RATE_SCALER     0
#define FP_SHIFT_AMOUNT 14 // 14 decimal places

// TO DO
// Butter coeffs
#define BUTTER_A0 15
#define BUTTER_A1 31
#define BUTTER_A2 15
#define BUTTER_B1 -31313
#define BUTTER_B2 14991
// a0 = 0.0009;
// a1 = 0.0019;
// a2 = 0.0009;
// b1 = -1.9112;
// b2 = 0.9150;

// #define BUTTER_A0 3
// #define BUTTER_A1 11
// #define BUTTER_A2 3
// #define BUTTER_B1 -31894
// #define BUTTER_B2 15534
// a0 = 0.0003460;
// a1 = 0.0006921;
// a2 = 0.0003460;
// b1 = -1.9467;
// b2 =  0.9481]

// Kalman coeffs
// #define P2PHI     16 // (0.001)
// #define Q2THETA   16 // (0.001)
#define P2PHI     32    // (0.002)
#define Q2THETA   32    // (0.002)

#define ROLL_C1   256 // (Shift 8 places)
#define ROLL_C1_SHIFT 8
#define PITCH_C1  256 // (Shift 8 places)
#define PITCH_C1_SHIFT 8
#define ROLL_C2 262144
#define ROLL_C2_SHIFT 18
#define PITCH_C2 262144
#define PITCH_C2_SHIFT 18

/* Rates and angles provided by filters */
int16_t phi, theta, psi;
int16_t p, q, r;

/* Rates and angles provided by filters in FIXED POINT */
int32_t phi_fp, theta_fp, psi_fp;
int32_t p_fp, q_fp, r_fp;

/* Previous rates and angles for Butterworth (in FIXED POINT) */
int32_t sphi_x1_fp, sphi_x2_fp, stheta_x1_fp, stheta_x2_fp, spsi_x1_fp, spsi_x2_fp;
int32_t sphi_y1_fp, sphi_y2_fp, stheta_y1_fp, stheta_y2_fp, spsi_y1_fp, spsi_y2_fp;
int32_t sp_x1_fp, sp_x2_fp, sq_x1_fp, sq_x2_fp, sr_x1_fp, sr_x2_fp;
int32_t sp_y1_fp, sp_y2_fp, sq_y1_fp, sq_y2_fp, sr_y1_fp, sr_y2_fp;

/* Kalman bias variables */
int32_t p_b_fp, q_b_fp;

void yaw_rate_filter(void);
void roll_filter(void);
void pitch_filter(void);
void reset_filters(void);
void yaw_sensoracq_and_filters(void);
void full_sensoracq_and_filters(void);
void hight_sensoracq_and_filters(void);

#endif // FILTERS_H__