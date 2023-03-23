// **************************************************************************
//
//               Demo program for APPS labs
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 02/2022
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         Main program for I2C bus
//
// **************************************************************************

#include <mbed.h>

#include "i2c-lib.h"
#include "si4735-lib.h"

//************************************************************************

// Direction of I2C communication
#define R	0b00000001
#define W	0b00000000

#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

DigitalOut g_led_PTA1( PTA1, 0 );
DigitalOut g_led_PTA2( PTA2, 0 );

DigitalIn g_but_PTC9( PTC9 );
DigitalIn g_but_PTC10( PTC10 );
DigitalIn g_but_PTC11( PTC11 );
DigitalIn g_but_PTC12( PTC12 );

int main( void )
{
	uint8_t l_S1, l_S2, l_RSSI, l_SNR, l_MULT, l_CAP;
	uint8_t l_ack = 0;

	printf( "K64F-KIT ready...\r\n" );

	i2c_init();

	// communication with 8 bit expander PCF8574

	// start communication
	i2c_start();

	// PCF8574 addressing
	// The address is composed from 3 parts!
	//l_ack = i2c_output( HWADR_PCF8574 | A012 | W );

	// Check l_ack! Return value must be 0!
	// ....

	//l_ack = i2c_output( Any_8_bit_value );
	// selected LEDs should light

	// stop communication
	i2c_stop();

	if ( ( l_ack = si4735_init() ) != 0 )
	{
		printf( "Initialization of SI4735 finish with error (%d)\r\n", l_ack );
		return 0;
	}
	else
		printf( "SI4735 initialized.\r\n" );

	printf( "\nTunig of radio station...\r\n" );

	// Required frequency in MHz * 100
	int l_freq = 10140; // Radiozurnal

	// Tuning of radio station
	i2c_start();
	l_ack |= i2c_output( SI4735_ADDRESS | W);
	l_ack |= i2c_output( 0x20 );			// FM_TUNE_FREQ
	l_ack |= i2c_output( 0x00 );			// ARG1
	l_ack |= i2c_output( l_freq >> 8 );		// ARG2 - FreqHi
	l_ack |= i2c_output( l_freq & 0xff );	// ARG3 - FreqLo
	l_ack |= i2c_output( 0x00 );			// ARG4
	i2c_stop();
	// Check l_ack!
	// if...

	// Tuning process inside SI4735
	wait_us( 100000 );
	printf( "... station tuned.\r\n\n" );

	// Example of reading of tuned frequency
	i2c_start();
	l_ack |= i2c_output( SI4735_ADDRESS | W );
	l_ack |= i2c_output( 0x22 );			// FM_TUNE_STATUS
	l_ack |= i2c_output( 0x00 );			// ARG1
	// repeated start
	i2c_start();
	// change direction of communication
	l_ack |= i2c_output( SI4735_ADDRESS | R );
	// read data
	l_S1 = i2c_input();
	i2c_ack();
	l_S2 = i2c_input();
	i2c_ack();
	l_freq = ( uint32_t ) i2c_input() << 8;
	i2c_ack();
	l_freq |= i2c_input();
	i2c_ack();
	l_RSSI = i2c_input();
	i2c_ack();
	l_SNR = i2c_input();
	i2c_ack();
	l_MULT = i2c_input();
	i2c_ack();
	l_CAP = i2c_input();
	i2c_nack();
	i2c_stop();

	if ( l_ack != 0 )
		printf( "Communication error!\r\n" );
	else
		printf( "Current tuned frequency: %d.%dMHz\r\n", l_freq / 100, l_freq % 100 );

	return 0;
}

