/*
    pkt_receiver.c
    Packet receiving state machine implementation
    Author: Jure Vidmar
*/
#include <stdio.h>
#include <stdint.h>
#include "pkt_receiver.h"
#include "packet.h"
#include "crc.h"
#include <assert.h>

void print_pkt_buf(PacketReceiver *p);  // TEMP DEBUG


/* STATE FUNCTIONS ----------------------------------------------------------*/
/*
    Idle state, look at the next byte in rx_queue. If START_BYTE detected, move to pkt_rx_type state
*/
void pkt_idle (PacketReceiver *p)
{
    // printf("Packet idle state!\n");

    // Don't do anything, if rx_queue is empty
    if (p->rx_queue->count <= 0) {
        return;
    }

    // Get one byte from rx_queue
    uint8_t byte = dequeue(p->rx_queue);

    if (byte == START_BYTE) {               // Move to the next state, if startbyte
        p->buf[p->ndx] = byte;              // Add byte to packet buffer
        p->ndx++;

        p->state = pkt_rx_type;
    } else {
        putchar(byte);                 // If received byte is not a packet, just print it
        reset_pkt_receiver(p);
    }

}


/*
    Type receiving state, parse incoming packet's type and determine type, subtype and length.
    Move to pkt_rx_bytes state, if everything is correct. In case of error or no type found -> back to idle
*/
void pkt_rx_type(PacketReceiver *p)
{
    // printf("pkt_rx_type state!\n");

    // Don't do anything, if rx_queue is empty
    if (p->rx_queue->count <= 0) {
        reset_pkt_receiver(p);
        return;
    }

    // Get one byte and parse it for type, subtype and length
    uint8_t byte = dequeue(p->rx_queue);
    p->type     = get_pkt_type(byte);
    p->subtype  = get_pkt_subtype(byte);
    p->length   = get_pkt_length(p->type, p->subtype);

    if (p->length > 0) {
        p->buf[p->ndx] = byte;          // Add byte to packet buffer
        p->ndx++;

        p->state = pkt_rx_bytes;        // Move to the next state
    } else {
        reset_pkt_receiver(p);
    }

}


/*
    Byte receiving state -> receives all bytes untill full packet is complete
*/
void pkt_rx_bytes(PacketReceiver *p)
{
    // printf("pkt_rx_bytes state!\n");

    // Receive bytes until number of received bytes (p->ndx) is equal to packet length
    while ((p->rx_queue->count > 0) && (p->ndx < p->length)) {
        p->buf[p->ndx] = dequeue(p->rx_queue);
        p->ndx++;
    }

    if (p->ndx == p->length) {
        p->state = pkt_crc_check;       // Move to next state
    } else {
        p->state = pkt_rx_bytes;
    }

}


/*
    CRC checking state -> calculates CRC, if okay move to next state. Else, reset pkt receiver to initial state.
*/
void pkt_crc_check(PacketReceiver *p)
{
    // printf("Crc checking state!\n");
    if (get_crc(p->buf, p->length) == 0) {
        p->state = pkt_action;
    } else {
        //printf("CRC failed! Printing buf: \n");
        //print_pkt_buf(p);
        reset_pkt_receiver(p);
    }

}


/*
    pkt_action - > Perform action based on received packet
*/
void pkt_action(PacketReceiver *p)
{
    switch (p->type) {

        case FLIGHT_TYPE:
            print_flight_pkt(p->buf);
            reset_pkt_receiver(p);
            break;

        case LOG_TYPE:
            if (p->subtype == LOG_RPLY_SUBTYPE) {
                read_log_rply_pkt(p->buf, log_buf, &logcur);
                reset_pkt_receiver(p);
            } else if (p->subtype == LOG_DONE_SUBTYPE) {
                write_log_file();
                reset_pkt_receiver(p);
            } else if (p->subtype == LOG_PRINT_SUBTYPE) {
                print_log_print_pkt(p->buf, p->length);
                reset_pkt_receiver(p);
            } else if (p->subtype == LOG_STATUS_SUBTYPE) {
                update_status_stuff(p->buf, &dronemode, &ae0, &ae1, &ae2, &ae3, &battery);
                reset_pkt_receiver(p);
            } else {
                printf("Unsupported log subtype received!\n");
                reset_pkt_receiver(p);
            }
            break;

        default:
            printf("Un-implemented packet type!\n");
            reset_pkt_receiver(p);
            return;
    }
}


/* HELPER FUNCTIONS ---------------------------------------------------------*/
/*
    Initialize packet receiver
*/
void init_pkt_receiver (PacketReceiver *p, queue *q)
{
    p->state = pkt_idle;
    p->length     = 0;
    p->type    = 0;
    p->subtype = 0;
    clear_buffer(p->buf, PKT_BUF_SIZE);
    p->ndx = 0;
    p->rx_queue = q;
}

/*
    Initialize logdata receiver
*/
void init_log (uint8_t *buf){
    logcur = 0;
    log_buf = buf;
}


/*
    Resets packet receiver's parameters (state, length, type, subtype, message buffer)
*/
void reset_pkt_receiver(PacketReceiver *p)
{
    p->state = pkt_idle;
    p->length  = 0;
    p->type    = 0;
    p->subtype = 0;
    clear_buffer(p->buf, PKT_BUF_SIZE);
    p->ndx = 0;
}


/* 
    Clears buffer, sets all elements to zero 
*/
void clear_buffer (uint8_t *buf, uint16_t len) 
{
    for (uint16_t i=0; i<len; i++) {
        *(buf + i) = 0;
    }
}


/*
    Extracts 16 bit number from uint8_t array
    *buf is the buffer to extract from
    offset is the first byte to extract from
*/
int16_t extract_value(uint8_t *buf, uint32_t offset)
{
    return  (int16_t)(buf[offset] << 8 | (int16_t) buf[offset+1]);
}


uint8_t get_pkt_type(uint8_t byte)
{   
    return ((byte >> 4) & 0x0F);        // Return upper nibble
}

uint8_t get_pkt_subtype(uint8_t byte)
{
    return (byte & 0x0F);               // Return lower nibble
}


/*
    Packet lengths lookup function
*/
uint8_t get_pkt_length(uint8_t type, uint8_t subtype) {

    switch (type) {
        case FLIGHT_TYPE:
            return FLIGHT_PKT_SIZE;
    
        case MODE_TYPE:
            return MODE_PKT_SIZE;

        case CONTROL_TYPE:
            return CONTROL_PKT_SIZE;

        case LOG_TYPE:
            if(subtype == LOG_RPLY_SUBTYPE)
                return LOG_RPLY_SIZE;
            else if(subtype == LOG_DONE_SUBTYPE)
                return LOG_DONE_SIZE;
            else if (subtype == LOG_PRINT_SUBTYPE) {
                return LOG_PRINT_SIZE;
            } else if (subtype == LOG_STATUS_SUBTYPE) {
                return LOG_STATUS_SIZE;
            } else {
                printf("Unknown log subtype!\n");
                return 0;
            }
        default:
            return 0;
    }

}


// TEMP DEBUG FUNC
void print_pkt_buf(PacketReceiver *p)
{
    if (p->length == 0) {
        printf("lenght 0!\n");
        return;
    }
    for (uint16_t i=0; i < p->length; i++) {
        printf("_ 0x%02X\t", p->buf[i]);
    }
    printf("\n");
}


/* Action functions ---------------------------------------------------------*/
/*
    Reads log reply packets and stores data into log buffer
    @pkt    -> packet buffer
    @logbuf -> buffer to store log data
    @indx   -> array starting index from where to write data 
*/
void read_log_rply_pkt(uint8_t *pkt, uint8_t *logbuf, uint32_t *indx)
{
    // Read the data from the packet, and store it in the log buffer
    for (int i = 0; i < LOG_RPLY_SIZE - 3; i++){
        logbuf[(*indx)++] = pkt[i+2];
    }
}

/*
    Writes logs to file (after LOG_)DONE pkt received)
*/
void write_log_file()
{   
    FILE *f = fopen("data.dat", "w" );
    if (f == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    fprintf(f, "r_time, r_mode,  r_ae[0], r_ae[1], r_ae[2], r_ae[3], r_phi, r_theta, r_psi, r_sp, r_sq, r_sr, r_sax, r_say, r_saz, r_p, r_q, r_r\n");


    for(uint32_t i=0; i<logcur; i+=LOG_RPLY_SIZE-3){
        
        long int r_time =   (((uint32_t)log_buf[i + LOG_TIME_POS -2]) << 0 |   ((uint32_t)log_buf[i + LOG_TIME_POS - 2 + 1]) << 8 |   ((uint32_t)log_buf[i + LOG_TIME_POS - 2 + 2]) << 16 |   ((uint32_t)log_buf[i + LOG_TIME_POS - 2 + 3]) << 24);
        uint8_t r_mode = log_buf[i + LOG_MODE_POS - 2];
        
        int16_t r_ae[4];
        r_ae[0] = extract_value(log_buf, i + LOG_AE_POS - 2 + 0);
        r_ae[1] = extract_value(log_buf, i + LOG_AE_POS - 2 + 2);
        r_ae[2] = extract_value(log_buf, i + LOG_AE_POS - 2 + 4);
        r_ae[3] = extract_value(log_buf, i + LOG_AE_POS - 2 + 6);

        int16_t r_phi   = extract_value(log_buf, i + LOG_PHI_POS - 2);
        int16_t r_theta = extract_value(log_buf, i + LOG_THETA_POS - 2);
        int16_t r_psi   = extract_value(log_buf, i + LOG_PSI_POS - 2);
        int16_t r_sp    = extract_value(log_buf, i + LOG_SP_POS - 2);
        int16_t r_sq    = extract_value(log_buf, i + LOG_SQ_POS - 2);
        int16_t r_sr    = extract_value(log_buf, i + LOG_SR_POS - 2);
        int16_t r_sax   = extract_value(log_buf, i + LOG_SAX_POS - 2);
        int16_t r_say   = extract_value(log_buf, i + LOG_SAY_POS - 2);
        int16_t r_saz   = extract_value(log_buf, i + LOG_SAZ_POS - 2);
        int16_t r_p    = extract_value(log_buf, i + LOG_P_POS - 2);
        int16_t r_q    = extract_value(log_buf, i + LOG_Q_POS - 2);
        int16_t r_r    = extract_value(log_buf, i + LOG_R_POS - 2);

        uint8_t output[128];
        sprintf((char*)output, "%10ld %3d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d",
                r_time, r_mode,  r_ae[0], r_ae[1], r_ae[2], r_ae[3], r_phi, r_theta, r_psi, r_sp, r_sq, r_sr, r_sax, r_say, r_saz, r_p, r_q, r_r);

        fprintf(f, "%s\n", output);
    }

    fclose(f);
    printf("Data written to file \n");

}


/*
    Prints the contents of log print packet
*/
void print_log_print_pkt(uint8_t *pkt, uint16_t len)
{
    printf("\nLog print pkt: \n");
    for (uint16_t i=TYPE_POS+1; i<len-1; i++) {
        putchar(*(pkt+i));
    }
    printf("\n");
}


void print_flight_pkt(uint8_t *pkt)
{
    printf("Flight pkt: ");
    for (uint16_t i=ROLL_POS; i<(FLIGHT_PKT_SIZE-1); i++) {
        printf("\t0x%02X", *(pkt+i));
    }
    printf("\n");
}


/* TEMP - gui status updating */
void update_status_stuff(uint8_t *pkt, uint8_t *mode, int16_t *ae0, int16_t *ae1, int16_t *ae2, int16_t *ae3, int16_t *bat)
{
    *mode = pkt[LOG_ST_MODE_POS];
    *ae0 = extract_value(pkt, LOG_ST_AE0_POS);
    *ae1 = extract_value(pkt, LOG_ST_AE1_POS);
    *ae2 = extract_value(pkt, LOG_ST_AE2_POS);
    *ae3 = extract_value(pkt, LOG_ST_AE3_POS);
    *bat = extract_value(pkt, LOG_ST_BAT_POS);
}
