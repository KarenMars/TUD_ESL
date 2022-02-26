 /*
 ============================================================================
 Name        : messageDecoder.c
 Author      : Snehal Jauhri
 Version     :
 Copyright   :
 Description : Decode messages from the rx queue
 ============================================================================
 */

#include "in4073.h"
#include "messageDecoder.h"
#include "packet.h"
#include "control.h"
#include "crc.h"
#include "datalogging.h"

/* global variables with requested mode from PC */
enum modes requested_mode = safe;
/* Decoding state */
enum message_states decode_state = newmessage;

/* Message buffer */
uint8_t msg_buff[BUFF_SIZE];
uint8_t* messageptr = msg_buff;

/* For decode state */
uint8_t message_data_count = 0;
uint8_t msgReceivedFlag = NO_MSG;

/* Inputs from PC */
int16_t roll = 0, pitch = 0, yaw = 0, lift = -INT16_MAX;

uint16_t flightdatacount = 0;

/*------------------------------------------------------------------
 * read_flightmsg -- Decodes the flight message
 *------------------------------------------------------------------
 */
void read_flightmsg(void)
{    
	roll = (int16_t)((msg_buff[ROLL_POS] << 8) | (msg_buff[ROLL_POS+1]));
	pitch = (int16_t)((msg_buff[PITCH_POS] << 8) | (msg_buff[PITCH_POS+1]));
	yaw = (int16_t)((msg_buff[YAW_POS] << 8) | (msg_buff[YAW_POS+1]));
	lift = (int16_t)((msg_buff[LIFT_POS] << 8) | (msg_buff[LIFT_POS+1]));
	
    flightdatacount++;  /* Keep count of flight data received for safety checks */
        
}

/*------------------------------------------------------------------
 * read_modemsg -- Decodes the mode message
 *------------------------------------------------------------------
 */
void read_modemsg(void)
{
	requested_mode = 0x0F & msg_buff[1];
}

/*------------------------------------------------------------------
 * read_controlmsg -- Decodes the control message
 *------------------------------------------------------------------
 */
void read_controlmsg(void)
{    
	P_yaw = (int16_t)((msg_buff[CTRL_P_POS] << 8) | (msg_buff[CTRL_P_POS+1]));
	P1_roll = (int16_t)((msg_buff[CTRL_P1_POS] << 8) | (msg_buff[CTRL_P1_POS+1]));
    P1_pitch = P1_roll;
	P2_roll = (int16_t)((msg_buff[CTRL_P2_POS] << 8) | (msg_buff[CTRL_P2_POS+1]));
    P2_pitch = P2_roll;	 
    PH_hight = (int16_t)((msg_buff[CTRL_PH_POS] << 8) | (msg_buff[CTRL_PH_POS+1]));   
}

/*------------------------------------------------------------------------------------
 * read_logmsg -- Decodes the log message
 *------------------------------------------------------------------------------------
 */
void read_logmsg(void)
{
    switch (0x0F & msg_buff[1]){
        case LOG_START_SUBTYPE:
            datalog_enable = true;
        break;
        case LOG_DONE_SUBTYPE:
            // This should never happen
        break;
        case LOG_RQST_SUBTYPE:
            requested_mode = sendinglog;            
        break;
        case LOG_RPLY_SUBTYPE:
            // This should never happen
        break;
    }    
}


/*------------------------------------------------------------------
 * message_get -- Check for and read messages from the PC
 *------------------------------------------------------------------
 */
void message_get(void)
{
    while(rx_queue.count)
    {
        *messageptr = dequeue(&rx_queue);
        
        switch(decode_state)
        {            
            case newmessage:
                /* Process new message */
                /* check start byte */           
                if(START_BYTE == *messageptr)
                {                                    
                    decode_state = decodeheader;
                    messageptr++;
                }
                break;
            case decodeheader:
                /* Decode message header to find type */
                if(FLIGHT_TYPE == (*messageptr >> 4))
                {                      
                    decode_state = flightmsg;
                    messageptr++;
                }
                else if(MODE_TYPE == (*messageptr >> 4))
                {
                    decode_state = modemsg;
                    messageptr++;
                }
                else if(CONTROL_TYPE == (*messageptr >> 4))
                {
                    decode_state = controlmsg;
                    messageptr++;
                }
                else if(LOG_TYPE == (*messageptr >> 4))
                {
                    decode_state = logmsg;
                    messageptr++;
                }
                else
                {
                    /* Invalid header */
                    decode_state = newmessage;
                    messageptr = msg_buff;
                    message_data_count = 0;
                }
                break;
            case flightmsg:
                /* Gather all bytes for flightmsg and the crc byte */
                message_data_count++;                          

                if(message_data_count == (FLIGHT_PKT_SIZE - 2))
                {
                    /* All bytes received, now check CRC */                    
                    //printf("Rcvd CRC: %#x \t CRC Result: %#x\n",*messageptr, calculatedCRC);
                    if (!get_crc(msg_buff,FLIGHT_PKT_SIZE))                  
                    {
                        msgReceivedFlag = FLIGHT_TYPE;                        
                    }
                    else
                    {
                        msgReceivedFlag = NO_MSG;
                    }
                    messageptr = msg_buff;
                    message_data_count = 0;
                    decode_state = newmessage;
                    return;
                }
                else
                {
                    messageptr++;
                }
                break;
            case modemsg:
                /* Gather all bytes for modemsg and the crc byte */
                message_data_count++;
                if(message_data_count == (MODE_PKT_SIZE - 2))
                {
                    /* All bytes received, now check CRC */                    
                    if (!get_crc(msg_buff,MODE_PKT_SIZE))                
                    {
                        msgReceivedFlag = MODE_TYPE;
                    }
                    else
                    {
                        msgReceivedFlag = NO_MSG;
                    }
                    messageptr = msg_buff;
                    message_data_count = 0;
                    decode_state = newmessage;
                    return;
                }
                else
                {
                    messageptr++;
                }
                break;                        
            case controlmsg:
                /* Gather all bytes for controlmsg and the crc byte */
                message_data_count++;
                if(message_data_count == (CONTROL_PKT_SIZE - 2))
                {
                    /* All bytes received, now check CRC */                    
                    if (!get_crc(msg_buff,CONTROL_PKT_SIZE))                     
                    {
                        msgReceivedFlag = CONTROL_TYPE;
                    }
                    else
                    {
                        msgReceivedFlag = NO_MSG;
                    }
                    messageptr = msg_buff;
                    message_data_count = 0;
                    decode_state = newmessage;
                    return;
                }
                else
                {
                    messageptr++;
                }
                break;    
            case logmsg:
                /* Gather all bytes for controlmsg and the crc byte */
                message_data_count++;
                switch (0x0F & msg_buff[1]){
                    case LOG_START_SUBTYPE:
                        if(message_data_count == (LOG_RQST_SIZE - 2))
                        {
                            /* All bytes received, now check CRC */                    
                            if (!get_crc(msg_buff,LOG_START_SIZE))                     
                            {
                                msgReceivedFlag = LOG_TYPE;
                            }
                            else
                            {
                                msgReceivedFlag = NO_MSG;
                            }
                            messageptr = msg_buff;
                            message_data_count = 0;
                            decode_state = newmessage;
                            return;
                        }
                        else
                        {
                            messageptr++;
                        }
                    break;
                    case LOG_DONE_SUBTYPE:
                        // This should never happen
                    break;
                    case LOG_RQST_SUBTYPE:
                        if(message_data_count == (LOG_RQST_SIZE - 2))
                        {
                            /* All bytes received, now check CRC */                    
                            if (!get_crc(msg_buff,LOG_RQST_SIZE))                     
                            {
                                msgReceivedFlag = LOG_TYPE;
                            }
                            else
                            {
                                msgReceivedFlag = NO_MSG;
                            }
                            messageptr = msg_buff;
                            message_data_count = 0;
                            decode_state = newmessage;
                            return;
                        }
                        else
                        {
                            messageptr++;
                        }  
                        break;
                    case LOG_RPLY_SUBTYPE:
                        // This should never happen
                    break;
                }
                
            default:
                decode_state = newmessage;
                messageptr = msg_buff;
                msgReceivedFlag = NO_MSG;
                message_data_count = 0;
        }        
    }    
}

/*------------------------------------------------------------------
 * message_react -- Act on messages from the PC
 *------------------------------------------------------------------
 */
void message_react(void)
{   
    switch (msgReceivedFlag)
    {
        case NO_MSG:
            break;
        case FLIGHT_TYPE:
            read_flightmsg();
            break;
        case MODE_TYPE:
            read_modemsg();
            break;
        case CONTROL_TYPE:
            read_controlmsg();
            break;
        case LOG_TYPE:
            read_logmsg();
            break;
        default:
            break;
    }        
    msgReceivedFlag = NO_MSG; // Reset flag       
    
}

