// **************************************************************************
//
//               Demo program for APPS labs
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 02/2022
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         FM/AM Radiomodule SI4735 with I2C interface
//
// **************************************************************************

#ifndef __SI4735_LIB_H
#define __SI4735_LIB_H

#define SI4735_ADDRESS	0x22

extern char g_fm_debug;

char si4735_init( void );

#endif // __SI4735_LIB_H

