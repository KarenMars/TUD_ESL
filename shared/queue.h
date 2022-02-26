#ifndef QUEUE_H
#define QUEUE_H
/*
    queue.h
    Queue definition, used on both control_station and drone
*/

#include <stdint.h>

#define QUEUE_SIZE 256

typedef struct {
	uint8_t  Data[QUEUE_SIZE];
	uint16_t first,last;
  	uint16_t count; 
} queue;


void init_queue(queue *q);
void enqueue(queue *q, char x);
char dequeue(queue *q);



#endif      // QUEUE_H
