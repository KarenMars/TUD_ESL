 /*
 ============================================================================
 Name        : datalogger.h
 Author      : Jelger
 Version     :
 Copyright   :
 Description : Performs the datalogging functions for quadruple copter
 ============================================================================
 */

#ifndef DATALOGGING_H
#define DATALOGGING_H

#include <stdbool.h>
#include <stdint.h>

/* variable to enable the datalogging*/
extern bool datalog_enable;

bool datalog_write(uint8_t mode);
bool datalog_read(uint8_t *data);

#endif      // DATALOG_H
