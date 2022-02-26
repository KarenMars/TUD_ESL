#ifndef PKT_RECEIVER_H
#define PKT_RECEIVER_H
/*
    pkt_receiver.h
    Packet receiving state machine header file
    Authors: Jure Vidmar
*/
#include <stdint.h>
#include "queue.h"


#define PKT_BUF_SIZE         256

uint8_t  *log_buf;
uint32_t logcur;

/* TEMP GUI STUFF */
int16_t ae0;
int16_t ae1;
int16_t ae2;
int16_t ae3;
int16_t battery;
uint8_t dronemode;

typedef struct PacketReceiver PacketReceiver;
struct PacketReceiver {

    void (*state)(PacketReceiver *p);   // Current state (function ptr)
    uint8_t length;                     // Packet length in bytes (total)
    uint8_t type;
    uint8_t subtype;
    uint8_t buf[PKT_BUF_SIZE];          // Buffer for currently incoming packet
    uint8_t ndx;                        // Pointer to the newest element in buffer (index)
    queue *rx_queue;                    // Pointer to the rx queue (raw serial bytes)

};

/* Helper functions ---------------------------------------------------------*/
void init_pkt_receiver (PacketReceiver *p, queue *q);
void init_log (uint8_t *buf);
void reset_pkt_receiver(PacketReceiver *p);
void clear_buffer (uint8_t *buf, uint16_t len);
int16_t extract_value(uint8_t *buf, uint32_t offset);
uint8_t get_pkt_type(uint8_t byte);
uint8_t get_pkt_subtype(uint8_t byte);
uint8_t get_pkt_length(uint8_t type, uint8_t subtype);

void print_log_pkt(uint8_t *pkt);
void print_log_print_pkt(uint8_t *pkt, uint16_t len);
void print_flight_pkt(uint8_t *pkt);

void read_log_rply_pkt(uint8_t *pkt, uint8_t *logbuf, uint32_t *logcursor);
void write_log_file();

void update_status_stuff(uint8_t *pkt, uint8_t *mode, int16_t *ae0, int16_t *ae1, int16_t *ae2, int16_t *ae3, int16_t *bat);

/* States -------------------------------------------------------------------*/
void pkt_idle(PacketReceiver *p);
void pkt_rx_type(PacketReceiver *p);
void pkt_rx_bytes(PacketReceiver *p);
void pkt_crc_check(PacketReceiver *p);
void pkt_action(PacketReceiver *p);         // Implemented separatelly for PC and drone


#endif  /* PKT_RECEIVER_H */