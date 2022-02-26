/*
    packet.c
    Implements the packet related functions -> PC SIDE
    Authors: Jure Vidmar, Jelger Lemmers
*/
#include "packet.h"
#include "queue.h"
#include "crc.h"
#include <stdio.h>

/*
	Splits 16-bit values into bytes, and inserts them into given array.
	MSB of the value is on buf[pos]
	LSB of the value is on buf[pos + 1]
*/
void insert_value(uint8_t *buf, uint8_t offset, uint16_t value)
{
    *(buf + offset)     = (uint8_t)(value >> 8);
    *(buf + offset + 1) = (uint8_t)(value & 0xFF);
}


/*
    Takes two 8-bit values and merges them into one 8-bit value
    It inserts merged value into given array (@buf) to the TYPE field
    @type    = upper 4-pits
    @subtype = lower 4 bits
*/
void insert_type(uint8_t *buf, uint8_t type, uint8_t subtype)
{
    uint8_t merge = ((type & 0x0F) << 4) | (subtype & 0x0F);
    *(buf + TYPE_POS) = merge; 
}


void create_flight_pkt(queue *qptr, int16_t roll, int16_t pitch, int16_t yaw, int16_t lift)
{

	uint8_t pkt[FLIGHT_PKT_SIZE] = {0};
	
	// Assemble the packet
	pkt[STARTB_POS] = START_BYTE;
    insert_type(&pkt[0], FLIGHT_TYPE, 0);           // &pkt[0]: zero needs to be there,
	insert_value(&pkt[0], ROLL_POS, roll);          // how to make this nicer? w/o compiler warns?
	insert_value(&pkt[0], PITCH_POS, pitch);
	insert_value(&pkt[0], YAW_POS, yaw);
	insert_value(&pkt[0], LIFT_POS, lift);
    /* Calculate CRC */
    pkt[FLIGHT_PKT_SIZE-1] = get_crc(pkt,FLIGHT_PKT_SIZE-1);	

	// Add the packet to the queue
	for (uint16_t i=0; i < FLIGHT_PKT_SIZE; i++) {
		enqueue(qptr, pkt[i]);
	}
}


void create_mode_pkt(queue *qptr, uint8_t mode)
{
    uint8_t pkt[MODE_PKT_SIZE];

    // Assemble the packet
    pkt[STARTB_POS] = START_BYTE;
    insert_type(&pkt[0], MODE_TYPE, mode);         // Merge type and subtype (mode) into one byte
    pkt[MODE_PKT_SIZE-1] = get_crc(pkt,MODE_PKT_SIZE-1);

    // Add the packet to the queue
    for (uint16_t i=0; i < MODE_PKT_SIZE; i++) {
		enqueue(qptr, pkt[i]);
	}
}


void create_ctrl_pkt(queue *qptr, int16_t yaw_P, int16_t P_1, int16_t P_2, int16_t P_H)
{
    uint8_t pkt[CONTROL_PKT_SIZE];

    // Assemble the packet
    pkt[STARTB_POS] = START_BYTE;
    insert_type(&pkt[0], CONTROL_TYPE, 0);
    insert_value(&pkt[0], CTRL_P_POS, yaw_P);
	insert_value(&pkt[0], CTRL_P1_POS, P_1);
	insert_value(&pkt[0], CTRL_P2_POS, P_2);
    insert_value(&pkt[0], CTRL_PH_POS, P_H);
    pkt[CONTROL_PKT_SIZE-1] = get_crc(pkt,CONTROL_PKT_SIZE-1);    

    // Add the packet to the queue
    for (uint16_t i=0; i < CONTROL_PKT_SIZE; i++) {
		enqueue(qptr, pkt[i]);           
	}

}


void create_log_start_pkt(queue *qptr)
{
    uint8_t pkt[LOG_START_SIZE];

    pkt[STARTB_POS] = START_BYTE;
    insert_type(&pkt[0], LOG_TYPE, LOG_START_SUBTYPE);
    pkt[LOG_START_SIZE-1] = get_crc(pkt, LOG_START_SIZE-1);

    // Add the packet to the queue
    for (uint8_t i=0; i < LOG_START_SIZE; i++) {
		enqueue(qptr, pkt[i]);
	}
}

void create_log_request_pkt(queue *qptr)
{
    uint8_t pkt[LOG_RQST_SIZE];

    pkt[STARTB_POS] = START_BYTE;
    insert_type(&pkt[0], LOG_TYPE, LOG_RQST_SUBTYPE);
    pkt[LOG_RQST_SIZE-1] = get_crc(pkt, LOG_RQST_SIZE-1);

    // Add the packet to the queue
    for (uint8_t i=0; i < LOG_RQST_SIZE; i++) {
		enqueue(qptr, pkt[i]);
	}
}


void create_log_rply_pkt(queue *qptr, uint8_t* data)
{
    uint8_t pkt[LOG_RPLY_SIZE];
    pkt[STARTB_POS] = START_BYTE;
    insert_type(&pkt[0], LOG_TYPE, LOG_RPLY_SUBTYPE);

    // Copy data into pkt 
    for (uint8_t i = TYPE_POS+1; i < LOG_RPLY_SIZE-1; i++) {
        pkt[i] = data[i-2];
    }
    pkt[LOG_RPLY_SIZE-1] = get_crc(pkt, LOG_RPLY_SIZE-1);

    // Add the packet to the queue
    for (uint8_t j=0; j < LOG_RPLY_SIZE; j++) {
        enqueue(qptr, pkt[j]);
	}
}


void create_log_done_pkt(queue *qptr)
{
    uint8_t pkt[LOG_DONE_SIZE];
    
    pkt[STARTB_POS] = START_BYTE;
    insert_type(&pkt[0], LOG_TYPE, LOG_DONE_SUBTYPE);
    pkt[LOG_DONE_SIZE-1] = get_crc(pkt, LOG_DONE_SIZE-1);

    // Add the packet to the queue
    for (uint8_t i=0; i < LOG_DONE_SIZE; i++) {
		enqueue(qptr, pkt[i]);
	}
}


/*
    Creates a log-type, print-subtype packet. s is pointer to string (can be string literal) to be sent
    Limited to LOG_PRINT_SIZE - 3 (STARTBYTE + TYPE + CRC)
*/
void create_log_print_pkt(queue *qptr, const char *s)
{
    uint8_t pkt[LOG_PRINT_SIZE] = {0};

    // Assemble the packet
    pkt[STARTB_POS] = START_BYTE;
    insert_type(&pkt[0], LOG_TYPE, LOG_PRINT_SUBTYPE);
    
    // Copy the string literal to the packet buffer
    uint8_t curr = TYPE_POS+1;
    uint8_t c;
    while (((c = *s) != '\0') && (curr < (LOG_PRINT_SIZE-1))) {       // Fill buffer until (not including) LOG_PRINT_SIZE -1 (for CRC)
        pkt[curr] = c;
        curr++;
        s++;
    }

    pkt[LOG_PRINT_SIZE-1] = get_crc(pkt, LOG_PRINT_SIZE-1);     // length specified without CRC byte


    // Add the packet to the queue
    for (uint16_t i=0; i < LOG_PRINT_SIZE; i++) {
		enqueue(qptr, pkt[i]);
	}
}


void create_log_status_pkt(queue *qptr, uint8_t mode_v, int16_t ae00, int16_t ae01, int16_t ae10, int16_t ae11, int16_t bat_v)
{
    uint8_t pkt[LOG_STATUS_SIZE] = {0};

    // Assemble the packet
    pkt[STARTB_POS] = START_BYTE;
    insert_type(&pkt[0], LOG_TYPE, LOG_STATUS_SUBTYPE);
    pkt[LOG_ST_MODE_POS] = mode_v;
    insert_value(&pkt[0], LOG_ST_AE0_POS, ae00);
    insert_value(&pkt[0], LOG_ST_AE1_POS, ae01);
    insert_value(&pkt[0], LOG_ST_AE2_POS, ae10);
    insert_value(&pkt[0], LOG_ST_AE3_POS, ae11);
    insert_value(&pkt[0], LOG_ST_BAT_POS, bat_v);
    pkt[LOG_STATUS_SIZE - 1] = get_crc(pkt, LOG_STATUS_SIZE - 1);

    // Add the packet to the queue
    for (uint16_t i=0; i < LOG_STATUS_SIZE; i++) {
		enqueue(qptr, pkt[i]);
	}    
}
