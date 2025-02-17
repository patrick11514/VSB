// **************************************************************************
//
//               Demo program for APPS labs
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 02/2022
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         I2C bus programming interface
//
// **************************************************************************
#ifndef __I2C_LIB_H
#define __I2C_LIB_H

#include <mbed.h>

// ***********************************************************************

// Pins of SCL and SDA lines
#define SCL_PIN PTC7
#define SDA_PIN PTC5

// Prototypes of all I2C functions

// I2C initialization
void i2c_init( void );

// Start communication on I2C
void i2c_start( void );

// Stop communication on I2C
void i2c_stop( void );

// Send ACK bit
void i2c_ack( void );

// Send NACK bit
void i2c_nack( void );

// get ACK
uint8_t i2c_get_ack( void );

// Send one byte to I2C bus
uint8_t i2c_output( uint8_t t_value );

// Receive one byte from I2C bus
uint8_t i2c_input( void );

#endif //  __I2C_LIB_H
