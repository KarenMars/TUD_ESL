 /*
 ============================================================================
 Name        : Datalogger.h
 Author      : Jelger
 Version     :
 Copyright   :
 Description : Performs the datalogging functions for quadruple copter
 ============================================================================
 */

#include "datalogging.h"
#include "in4073.h"
#include "packet.h"
#include "filters.h"

bool datalog_enable = false;
uint32_t fl_address = 0;
uint32_t rd_address = 0;

bool datalog_write(uint8_t mode)
{
    uint8_t flashLog[LOG_RPLY_SIZE-3];
    // Before first write can happen, flash must be erased
    if(fl_address == 0){
        flash_chip_erase();
    }
    // Use value i to remove offset from start of packet to start of data
    uint32_t i = 2;

    //Helper variable to only use get_time_us() once
    uint32_t r_time = get_time_us();
    flashLog[LOG_TIME_POS -i  ]    = (r_time & 0x000000FF) >> 0;
    flashLog[LOG_TIME_POS -i+1]    = (r_time & 0x0000FF00) >> 8;
    flashLog[LOG_TIME_POS -i+2]    = (r_time & 0x00FF0000) >> 16;
    flashLog[LOG_TIME_POS -i+3]    = (r_time & 0xFF000000) >> 24;
    flashLog[LOG_MODE_POS -i  ]    = mode;
    flashLog[LOG_AE_POS   -i  ]    = (ae[0] & 0xFF00) >> 8;
    flashLog[LOG_AE_POS   -i+1]    = (ae[0] & 0x00FF) >> 0;
    flashLog[LOG_AE_POS   -i+2]    = (ae[1] & 0xFF00) >> 8;
    flashLog[LOG_AE_POS   -i+3]    = (ae[1] & 0x00FF) >> 0;
    flashLog[LOG_AE_POS   -i+4]    = (ae[2] & 0xFF00) >> 8;
    flashLog[LOG_AE_POS   -i+5]    = (ae[2] & 0x00FF) >> 0;
    flashLog[LOG_AE_POS   -i+6]    = (ae[3] & 0xFF00) >> 8;
    flashLog[LOG_AE_POS   -i+7]    = (ae[3] & 0x00FF) >> 0;
    flashLog[LOG_PHI_POS  -i  ]    = (phi   & 0xFF00) >> 8;
    flashLog[LOG_PHI_POS  -i+1]    = (phi   & 0x00FF) >> 0;
    flashLog[LOG_THETA_POS-i  ]    = (theta & 0xFF00) >> 8;
    flashLog[LOG_THETA_POS-i+1]    = (theta & 0x00FF) >> 0;
    flashLog[LOG_PSI_POS  -i  ]    = (psi   & 0xFF00) >> 8;
    flashLog[LOG_PSI_POS  -i+1]    = (psi   & 0x00FF) >> 0;
    flashLog[LOG_SP_POS   -i  ]    = (sp    & 0xFF00) >> 8;
    flashLog[LOG_SP_POS   -i+1]    = (sp    & 0x00FF) >> 0;
    flashLog[LOG_SQ_POS   -i  ]    = (sq    & 0xFF00) >> 8;
    flashLog[LOG_SQ_POS   -i+1]    = (sq    & 0x00FF) >> 0;
    flashLog[LOG_SR_POS   -i  ]    = (sr    & 0xFF00) >> 8;
    flashLog[LOG_SR_POS   -i+1]    = (sr    & 0x00FF) >> 0;
    flashLog[LOG_SAX_POS  -i  ]    = (sax   & 0xFF00) >> 8;
    flashLog[LOG_SAX_POS  -i+1]    = (sax   & 0x00FF) >> 0;
    flashLog[LOG_SAY_POS  -i  ]    = (say   & 0xFF00) >> 8;
    flashLog[LOG_SAY_POS  -i+1]    = (say   & 0x00FF) >> 0;
    flashLog[LOG_SAZ_POS  -i  ]    = (saz   & 0xFF00) >> 8;
    flashLog[LOG_SAZ_POS  -i+1]    = (saz   & 0x00FF) >> 0;
    
    flashLog[LOG_P_POS   -i  ]    = (p    & 0xFF00) >> 8;
    flashLog[LOG_P_POS   -i+1]    = (p    & 0x00FF) >> 0;
    flashLog[LOG_Q_POS   -i  ]    = (q    & 0xFF00) >> 8;
    flashLog[LOG_Q_POS   -i+1]    = (q    & 0x00FF) >> 0;
    flashLog[LOG_R_POS   -i  ]    = (r    & 0xFF00) >> 8;
    flashLog[LOG_R_POS   -i+1]    = (r    & 0x00FF) >> 0;    

    if(flash_write_bytes(fl_address, flashLog, LOG_RPLY_SIZE-3)){
        fl_address += LOG_RPLY_SIZE-3;
    }
    else{
        return false;
    }
    return true;
}


bool datalog_read(uint8_t *data)            // Returns one packet log message each time
{
    if(fl_address > rd_address + LOG_RPLY_SIZE-3){
        flash_read_bytes(rd_address, data, LOG_RPLY_SIZE-3);        
        rd_address+= LOG_RPLY_SIZE-3;
        return 1;
    }
    return 0;
}
