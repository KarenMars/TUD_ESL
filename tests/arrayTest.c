/*
    How to properly assemble uint8 and uint16 stuff
*/
#include <stdio.h>
#include <stdint.h>


void insert_value (uint8_t (*buf)[20], uint8_t pos, uint16_t value)
{
    uint8_t *ptr = buf[0];                      // Cast pointer from <uint8_t (*)[20]> into <uint8_t *>
    *(ptr+pos)   = (uint8_t)(value >> 8);
    *(ptr+pos+1) = (uint8_t)(value & 0xFF);
}

void insert_value2(uint8_t *buf, uint8_t offset, uint16_t value)
{
    *(buf + offset)     = (uint8_t)(value >> 8);
    *(buf + offset + 1) = (uint8_t)(value & 0xFF);
}


uint8_t queue[20];

int main(void)
{

    // Init queue
    for (int i=0; i<20; i++) {
        queue[i] = 0;
    }

    // How do you enter a 16bit variable, or a signed variable into it correctly?
    uint16_t valA = 120;
    uint8_t *ptr = &queue[0];
    insert_value2(&queue[0], 0, valA);

    // How do you enter a signed variable?
    int16_t valB = -120;

    insert_value2(ptr, 2, valB);


    // Retrieving data
    uint16_t recA = (uint16_t)((queue[0] << 8) | (queue[1] & 0xFF));
    int16_t recB = (int16_t)((queue[2] << 8) | (queue[3] & 0xFF));

    printf("Value A: %d\t Value B: %d\n", valA, valB);
    printf("Received A: %d\t Received B: %d\n", recA, recB);

    return 0;
}