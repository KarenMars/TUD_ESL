/*
    pkt_receiver_test.c
*/
#include "pkt_receiver.h"
#include "queue.h"
#include "packet.h"
#include "pkt_receiver.h"
#include <stdio.h>
#include <assert.h>

queue rx_queue;


void test_pkt_action()
{
    // Init rx queue and packet receiver
    init_queue(&rx_queue);
    PacketReceiver pkt_rx;
    init_pkt_receiver(&pkt_rx, &rx_queue);

    create_log_packet(&rx_queue, LOG_RESP_SUBTYPE, 8, 9);
    
    // Execute just enough steps, to collect the whole packet
    for (int i=0; i<3; i++) {
        (*pkt_rx.state)(&pkt_rx);
    }

    assert(pkt_rx.state == pkt_crc_check);
    (*pkt_rx.state)(&pkt_rx);
    assert(pkt_rx.state == pkt_action);
    (*pkt_rx.state)(&pkt_rx);
    (*pkt_rx.state)(&pkt_rx);

    printf("Action test passed!\n");

}


void test_pkt_rx_crc()
{
    // yolo
}


/*
    Test byte receiving
*/
void test_pkt_rx_bytes()
{
    // Init rx queue and packet receiver
    init_queue(&rx_queue);
    PacketReceiver pkt_rx;
    init_pkt_receiver(&pkt_rx, &rx_queue);

    create_flight_packet(&rx_queue, 6, 7, 8, 9);
    
    // Execute just enough steps, to collect the whole packet
    for (int i=0; i<4; i++) {
        (*pkt_rx.state)(&pkt_rx);
    }

    assert(pkt_rx.state == pkt_crc_check);      // After packet is collected, should be in CRC state
    assert(pkt_rx.ndx == FLIGHT_PKT_SIZE);
    assert(pkt_rx.type == FLIGHT_TYPE);
    assert(pkt_rx.subtype == 0x00);
    assert(pkt_rx.length == FLIGHT_PKT_SIZE);
}


/* Test type state */
void test_pkt_type()
{
    // Init rx queue and packet receiver
    init_queue(&rx_queue);
    PacketReceiver pkt_rx;
    init_pkt_receiver(&pkt_rx, &rx_queue);

    // If rx queue is empty, state should not change
    (*pkt_rx.state)(&pkt_rx);
    assert(pkt_rx.state == pkt_idle);

    create_flight_packet(&rx_queue, 6, 7, 8, 9);

    // Do three steps
    for (int i=0; i < 3; i++) {
        (*pkt_rx.state)(&pkt_rx);
    }

    assert(pkt_rx.length == FLIGHT_PKT_SIZE);
    assert(pkt_rx.type == FLIGHT_TYPE);
    assert(pkt_rx.subtype == 0x00);

    printf("Type test passed!\n");
}


/*
    Tests idle state
*/
void test_pkt_idle()
{

    // Init rx queue and packet receiver
    init_queue(&rx_queue);
    PacketReceiver pkt_rx;
    init_pkt_receiver(&pkt_rx, &rx_queue);

    // If rx queue is empty, state should not change
    (*pkt_rx.state)(&pkt_rx);
    assert(pkt_rx.state == pkt_idle);

    // If there is a random, not-startbyte, state should not change
    enqueue(&rx_queue, 0x66);
    (*pkt_rx.state)(&pkt_rx);
    assert(pkt_rx.state == pkt_idle);

    // If there's startbyte, state should change
    enqueue(&rx_queue, START_BYTE);
    (*pkt_rx.state)(&pkt_rx);
    assert(pkt_rx.state == pkt_rx_type);

    printf("Test pkt idle test passed!\n");
}


/*
    Tests initialization of packet receiver structure
*/
void test_pkt_receiver_init() 
{
    // Init rx queue and packet receiver
    init_queue(&rx_queue);
    PacketReceiver pkt_rx;
    init_pkt_receiver(&pkt_rx, &rx_queue);

    // TEST
    assert(pkt_rx.state == pkt_idle);
    assert(pkt_rx.length == 0);
    assert(pkt_rx.type == 0);
    assert(pkt_rx.subtype == 0);
    assert(pkt_rx.buf[0] == 0);
    assert(pkt_rx.buf[PKT_BUF_SIZE-1] == 0);
    assert(pkt_rx.rx_queue->count == 0);
    assert(pkt_rx.ndx == 0);

    printf("test_pkt_receiver_init test passed!\n");
}


int main (int argc, char **argv)
{
    // test_pkt_receiver_init();
    // test_pkt_idle();
    // test_pkt_type();
    // test_pkt_rx_bytes();
    test_pkt_action();

    
    return 0;
}