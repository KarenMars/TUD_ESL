 /*
 ============================================================================
 Name        : messageDecoder.h
 Author      : Snehal Jauhri
 Version     :
 Copyright   :
 Description : Decode messages from the rx queue
 ============================================================================
 */

#ifndef MESSAGEDECODER_H
#define MESSAGEDECODER_H

#include "statemachine.h"

#define NO_MSG        0x00 // When no message received
#define BUFF_SIZE     50

extern enum modes requested_mode;
extern uint8_t msgReceivedFlag;
extern uint16_t flightdatacount;

/* Inputs from PC */
extern int16_t roll, pitch, yaw, lift;

/* States for message decoding */
enum message_states
{
    newmessage,
    decodeheader,
    flightmsg,
    modemsg,
    controlmsg,
    logmsg    
};

void message_get(void); // Check for and read messages from the PC
void message_react(void); // Act on messages from the PC
void read_flightmsg(void);
void read_modemsg(void);
void read_controlmsg(void);
void read_logmsg(void);

#endif      // MESSAGEDECODER_H