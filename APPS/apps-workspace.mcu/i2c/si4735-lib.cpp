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

#include "i2c-lib.h"
#include "si4735-lib.h"

#define INIT_DELAY 100			// ms

char g_fm_debug = 0;

static void fm_print( const char *t_msg )
{
	if ( g_fm_debug )
	{ /*printf( t_msg );*/
	}
}

static char fm_check( char t_ack )
{
	if ( t_ack != 1 )
		fm_print( "- OK\n" );
	else
		fm_print( "- Error\n" );

	return t_ack;
}

char si4735_init( void )
{
	char l_ack = 0;
	fm_print( "----- Si4735 Init Started ------\n" );

	fm_print( " Power Down                " );
	i2c_start();
	l_ack |= i2c_output( SI4735_ADDRESS );
	l_ack |= i2c_output( 0x11 );			// power down
	i2c_stop();

	wait_us( INIT_DELAY * 100 );
	if ( fm_check( l_ack ) )
		return 1;

	fm_print( " Init FM                   " );
	i2c_start();
	l_ack |= i2c_output( SI4735_ADDRESS );
	l_ack |= i2c_output( 0x01 );			// power up
	l_ack |= i2c_output( 0x10 );			// set crystal 32KHz
	l_ack |= i2c_output( 0x05 );			// set analog output
	i2c_stop();

	wait_us( INIT_DELAY * 1000 );
	if ( fm_check( l_ack ) )
		return 2;

	fm_print( " FM antenna input          " );
	i2c_start();
	l_ack |= i2c_output( SI4735_ADDRESS );
	l_ack |= i2c_output( 0x12 );			// set property
	l_ack |= i2c_output( 0x00 );			// set property

	l_ack |= i2c_output( 0x11 );
	l_ack |= i2c_output( 0x07 );

	l_ack |= i2c_output( 0x00 );
	l_ack |= i2c_output( 0x00 );
	i2c_stop();

	wait_us( INIT_DELAY * 1000 );
	if ( fm_check( l_ack ) )
		return 3;

	fm_print( " FM Deemphasis             " );
	i2c_start();
	l_ack |= i2c_output( SI4735_ADDRESS );
	l_ack |= i2c_output( 0x12 );			// set property
	l_ack |= i2c_output( 0x00 );			// set property

	l_ack |= i2c_output( 0x11 );
	l_ack |= i2c_output( 0x00 );

	l_ack |= i2c_output( 0x00 );
	l_ack |= i2c_output( 0x01 );
	i2c_stop();

	wait_us( INIT_DELAY * 1000 );
	if ( fm_check( l_ack ) )
		return 4;

	fm_print( " Freq. Spacing 5kHz        " );
	i2c_start();
	l_ack |= i2c_output( SI4735_ADDRESS );
	l_ack |= i2c_output( 0x12 );			// set property
	l_ack |= i2c_output( 0x00 );			// set property

	l_ack |= i2c_output( 0x14 );
	l_ack |= i2c_output( 0x02 );

	l_ack |= i2c_output( 0x00 );
	l_ack |= i2c_output( 0x05 );
	i2c_stop();

	wait_us( INIT_DELAY * 1000 );
	if ( fm_check( l_ack ) )
		return 5;

	fm_print( " Init RDS( 1 )               " );
	i2c_start();
	l_ack |= i2c_output( SI4735_ADDRESS );
	l_ack |= i2c_output( 0x12 ); //set property
	l_ack |= i2c_output( 0x00 ); //set property

	l_ack |= i2c_output( 0x15 ); //RDS_CONFIG
	l_ack |= i2c_output( 0x02 ); //RDS_CONFIG

	l_ack |= i2c_output( 0xAA ); //err.corectable
	l_ack |= i2c_output( 0x01 ); //err.corectable
	i2c_stop();

	wait_us( INIT_DELAY * 1000 );
	if ( fm_check( l_ack ) )
		return 6;

	fm_print( " Init RDS( 2 )               " );
	i2c_start();
	l_ack |= i2c_output( SI4735_ADDRESS );
	l_ack |= i2c_output( 0x12 ); //set property
	l_ack |= i2c_output( 0x00 ); //set property

	l_ack |= i2c_output( 0x15 ); //RDS_CONFIG
	l_ack |= i2c_output( 0x00 ); //RDS_CONFIG

	l_ack |= i2c_output( 0x00 ); //err.corectable
	l_ack |= i2c_output( 0x00 ); //err.corectable
	i2c_stop();

	wait_us( INIT_DELAY * 1000 );
	if ( fm_check( l_ack ) )
		return 7;

	fm_print( " Init RDS( 3 )               " );
	i2c_start();
	l_ack |= i2c_output( SI4735_ADDRESS );
	l_ack |= i2c_output( 0x12 ); //set property
	l_ack |= i2c_output( 0x00 ); //set property

	l_ack |= i2c_output( 0x15 ); //RDS_CONFIG
	l_ack |= i2c_output( 0x01 ); //RDS_CONFIG

	l_ack |= i2c_output( 0x00 ); //err.corectable
	l_ack |= i2c_output( 0x00 ); //err.corectable
	i2c_stop();

	wait_us( INIT_DELAY * 1000 );
	if ( fm_check( l_ack ) )
		return 8;

	fm_print( " Set property              " );
	i2c_start();
	l_ack |= i2c_output( SI4735_ADDRESS );
	l_ack |= i2c_output( 0x20 );
	l_ack |= i2c_output( 0x00 );

	l_ack |= i2c_output( 0x27 );
	l_ack |= i2c_output( 0x10 );

	l_ack |= i2c_output( 0x00 );
	l_ack |= i2c_output( 0x00 );
	i2c_stop();

	wait_us( INIT_DELAY * 1000 );
	if ( fm_check( l_ack ) )
		return 9;

	i2c_start();
	i2c_output( SI4735_ADDRESS );
	i2c_output( 0x12 );
	i2c_output( 0x00 );
	i2c_output( 0x40 );
	i2c_output( 0x00 );
	i2c_output( 0x00 );
	i2c_output( 0x20 );					// hlasitost 32
	i2c_stop();

	fm_print( "----- Si4735 Init Done ---------\n" );

	return 0;
}
