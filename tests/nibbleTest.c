/*
    How to properly assemble two 4-bit values into a byte
*/
#include <stdio.h>
#include <stdint.h>

uint8_t queue[20];

void insert_value(uint8_t *buf, uint8_t offset, uint16_t value)
{
    *(buf + offset)     = (uint8_t)(value >> 8);
    *(buf + offset + 1) = (uint8_t)(value & 0xFF);
}


int main(void)
{

    // Init queue
    for (int i=0; i<20; i++) {
        queue[i] = 0;
    }

    uint8_t type = 5;
    uint8_t subtype = 15;
    
    printf("Type: %d\t subtype: %d\n", type, subtype);

    // Merge
    uint8_t merge = ((type & 0x0F) << 4) | (subtype & 0x0F);
    
    // Retrieve
    uint8_t typeRec    = ((merge & 0xF0) >> 4);
    uint8_t subtypeRec = (merge & 0x0F);

    printf("Received type: %d\t subtype: %d\n", typeRec, subtypeRec);
    
    return 0;
}