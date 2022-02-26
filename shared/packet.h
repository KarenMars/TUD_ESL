#ifndef PACKET_H
#define PACKET_H
/*
    packet.h
    Communication packet structure definitions
    Authors: Jure Vidmar, Jelger Lemmers
*/

#include <stdint.h>
#include "queue.h"

/* Bit positions -----------------------------------------------------------*/
// Common
#define STARTB_POS          0
#define TYPE_POS            1       // High nibble = type, low nibble = subtype
// CRC == one byte, always the last byte

// Flight packet
#define ROLL_POS			2
#define PITCH_POS			4
#define YAW_POS				6
#define LIFT_POS			8

// Mode packet
// no need

// Control packet
#define CTRL_P_POS          2
#define CTRL_P1_POS         4
#define CTRL_P2_POS         6
#define CTRL_PH_POS         8

// Log status packet
#define LOG_ST_MODE_POS     2
#define LOG_ST_AE0_POS      3
#define LOG_ST_AE1_POS      5
#define LOG_ST_AE2_POS      7
#define LOG_ST_AE3_POS      9
#define LOG_ST_BAT_POS      11

// Log reply packet contents
#define LOG_TIME_POS         2
#define LOG_MODE_POS         6
#define LOG_AE_POS           7
#define LOG_PHI_POS          15
#define LOG_THETA_POS        17
#define LOG_PSI_POS          19
#define LOG_SP_POS           21
#define LOG_SQ_POS           23
#define LOG_SR_POS           25
#define LOG_SAX_POS          27
#define LOG_SAY_POS          29
#define LOG_SAZ_POS          31
#define LOG_P_POS            33
#define LOG_Q_POS            35
#define LOG_R_POS            37


/* Packet values -----------------------------------------------------------*/
// Common
#define START_BYTE          0x55

// Flight packet
#define FLIGHT_TYPE			0xF
#define FLIGHT_PKT_SIZE		11

// Mode packet
#define MODE_TYPE           0xD
#define MODE_PKT_SIZE       3

#define MODE_SAFE           0
#define MODE_PANIC          1
#define MODE_MANUAL         2
#define MODE_CALIBRATION    3
#define MODE_YAW_CTRL       4
#define MODE_FULL_CTRL      5
#define MODE_RAW            6
#define MODE_DMP            7
#define MODE_HEIGHT_CTRL    8
#define MODE_WIRELESS       9

// Control packet
#define CONTROL_TYPE		0xC
#define CONTROL_PKT_SIZE    11

// Log packets
#define LOG_TYPE            0xE

#define LOG_START_SUBTYPE   0x0
#define LOG_RQST_SUBTYPE    0x1
#define LOG_RPLY_SUBTYPE    0x2
#define LOG_DONE_SUBTYPE    0x3
#define LOG_PRINT_SUBTYPE   0x4
#define LOG_STATUS_SUBTYPE  0x5

#define LOG_START_SIZE       3
#define LOG_RQST_SIZE        3
#define LOG_RPLY_SIZE        40
#define LOG_DONE_SIZE        3
#define LOG_PRINT_SIZE       45
#define LOG_STATUS_SIZE      14

/* Function definitions -----------------------------------------------------*/
void create_flight_pkt(queue *qptr, int16_t roll, int16_t pitch, int16_t yaw, int16_t lift);
void create_mode_pkt(queue *qptr, uint8_t mode);
void create_ctrl_pkt(queue *qptr, int16_t yaw_P, int16_t P1, int16_t P2, int16_t PH);

void create_log_start_pkt(queue *qptr);
void create_log_request_pkt(queue *qptr);
void create_log_rply_pkt(queue *qptr, uint8_t* data);
void create_log_done_pkt(queue *qptr);

void create_log_print_pkt(queue *qptr, const char *str);
void create_log_status_pkt(queue *qptr, uint8_t mode, int16_t ae0, int16_t ae1, int16_t ae2, int16_t ae3, int16_t bat);

void insert_value(uint8_t *buf, uint8_t offset, uint16_t value);
void create_header(uint8_t type, uint8_t subtype);


#endif      /* PACKET_H */