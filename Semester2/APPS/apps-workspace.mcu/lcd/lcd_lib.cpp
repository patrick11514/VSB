// **************************************************************************
//
//               Demo program for labs
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 02/2022
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         Programming interface for LCD module
//
// **************************************************************************

#include "mbed.h"
#include "lcd_lib.h"

#define SPI_FREQ	10000000		// SPI speed

DigitalOut _g_cs( PTC0, 0 );		// chip select
DigitalOut _g_reset( PTC1, 0 );		// reset
DigitalOut _g_rs( PTC2, 0 );		// command/data
DigitalOut _g_bl( PTC3, 0 );		// backlight

// **************************************************************************
// internal functions

#define Nop() { asm( "nop" ); }

// SPI interface
static SPI _g_spi( PTD2, PTD3, PTD1 );

// send command to LCD controller
void lcd_write_command( uint8_t t_cmd )
{
	_g_cs = 0;
	Nop();
	_g_rs = 0;
	_g_spi.write( t_cmd );
	_g_cs = 1;
}

// send one byte to LCD controller
void lcd_write_data( uint8_t t_data )
{
	_g_cs = 0;
	Nop();
	_g_rs = 1;
	_g_spi.write( t_data );
	_g_cs = 1;
}

// send two bytes to LCD controller
void lcd_write_data_16( uint16_t t_data )
{
	uint8_t buf[ 2 ] = { ( uint8_t ) ( t_data >> 8 ), ( uint8_t ) t_data };
	_g_cs = 0;
	Nop();
	_g_rs = 1;
	_g_spi.write( ( char * ) buf, 2, nullptr, 0 );
	_g_cs = 1;
}

void lcd_write_data_buf( uint8_t * t_data, uint32_t t_len )
{
	_g_cs = 0;
	Nop();
	_g_rs = 1;
	_g_spi.write( ( char * ) t_data, t_len, nullptr, 0 );
	_g_cs = 1;
}

// **************************************************************************
// LCD programming interface

// draw one pixel on LCD screen
void lcd_put_pixel( int32_t t_x, int32_t t_y, uint16_t t_color )
{
	lcd_write_command( 0x2A );
	lcd_write_data_16( t_x );
	lcd_write_data_16( t_x );

	lcd_write_command( 0x2B );
	lcd_write_data_16( t_y );
	lcd_write_data_16( t_y );

	lcd_write_command( 0x2C );
	lcd_write_data_16( t_color );
}

// HW reset of LCD controller
void lcd_reset()
{
	_g_reset = 0;
	wait_us( 100000 );
	_g_reset = 1;
	wait_us( 100000 );
}

// LCD controller initialization
void lcd_init()
{
	// init SPI interface
	_g_spi.format( 8, 0 );
	_g_spi.frequency( SPI_FREQ );

	// HW reset of LCD controller
	lcd_reset();

	// backlight ON
	_g_bl = 0;

	// initialization sequence... see documentation
	lcd_write_command( 0xCB );
	lcd_write_data( 0x39 );
	lcd_write_data( 0x2C );
	lcd_write_data( 0x00 );
	lcd_write_data( 0x34 );
	lcd_write_data( 0x02 );

	lcd_write_command( 0xCF );
	lcd_write_data( 0x00 );
	lcd_write_data( 0XC1 );
	lcd_write_data( 0X30 );

	lcd_write_command( 0xE8 );
	lcd_write_data( 0x85 );
	lcd_write_data( 0x00 );
	lcd_write_data( 0x78 );

	lcd_write_command( 0xEA );
	lcd_write_data( 0x00 );
	lcd_write_data( 0x00 );

	lcd_write_command( 0xED );
	lcd_write_data( 0x64 );
	lcd_write_data( 0x03 );
	lcd_write_data( 0X12 );
	lcd_write_data( 0X81 );

	lcd_write_command( 0xF7 );
	lcd_write_data( 0x20 );

	lcd_write_command( 0xC0 );    	//Power control
	lcd_write_data( 0x23 );   		//VRH[5:0]

	lcd_write_command( 0xC1 );    	//Power control
	lcd_write_data( 0x10 );   		//SAP[2:0];BT[3:0]

	lcd_write_command( 0xC5 );    	//VCM control
	lcd_write_data( 0x3e );   		//Contrast
	lcd_write_data( 0x28 );

	lcd_write_command( 0xC7 );    	//VCM control2
	lcd_write_data( 0x86 );   		//--

	lcd_write_command( 0x36 );    	// Memory Access Control
	//lcd_write_data(  0x48 );
	lcd_write_data( 0xE8 );

	lcd_write_command( 0x2A );
	lcd_write_data_16( 0 );
	lcd_write_data_16( LCD_WIDTH - 1 );

	lcd_write_command( 0x2B );
	lcd_write_data_16( 0 );
	lcd_write_data_16( LCD_HEIGHT - 1 );

	lcd_write_command( 0x3A );
	lcd_write_data( 0x55 );

	lcd_write_command( 0xB1 );
	lcd_write_data( 0x00 );
	lcd_write_data( 0x18 );

	lcd_write_command( 0xB6 );    	// Display Function Control
	lcd_write_data( 0x08 );
	lcd_write_data( 0x82 );
	lcd_write_data( 0x27 );
	/*
	 lcd_write_command( 0xF2 );    	// 3Gamma Function Disable
	 lcd_write_data( 0x00 );

	 lcd_write_command( 0x26 );    	//Gamma curve selected
	 lcd_write_data( 0x01 );

	 lcd_write_command( 0xE0 );    	//Set Gamma
	 lcd_write_data( 0x0F );
	 lcd_write_data( 0x31 );
	 lcd_write_data( 0x2B );
	 lcd_write_data( 0x0C );
	 lcd_write_data( 0x0E );
	 lcd_write_data( 0x08 );
	 lcd_write_data( 0x4E );
	 lcd_write_data( 0xF1 );
	 lcd_write_data( 0x37 );
	 lcd_write_data( 0x07 );
	 lcd_write_data( 0x10 );
	 lcd_write_data( 0x03 );
	 lcd_write_data( 0x0E );
	 lcd_write_data( 0x09 );
	 lcd_write_data( 0x00 );

	 lcd_write_command( 0XE1 );    	//Set Gamma
	 lcd_write_data( 0x00 );
	 lcd_write_data( 0x0E );
	 lcd_write_data( 0x14 );
	 lcd_write_data( 0x03 );
	 lcd_write_data( 0x11 );
	 lcd_write_data( 0x07 );
	 lcd_write_data( 0x31 );
	 lcd_write_data( 0xC1 );
	 lcd_write_data( 0x48 );
	 lcd_write_data( 0x08 );
	 lcd_write_data( 0x0F );
	 lcd_write_data( 0x0C );
	 lcd_write_data( 0x31 );
	 lcd_write_data( 0x36 );
	 lcd_write_data( 0x0F );
	 */
	lcd_write_command( 0x11 );    	//Exit Sleep
	wait_us( 120000 );

	lcd_write_command( 0x29 );    	//Display on
	lcd_write_command( 0x2c );
	uint16_t l_buf[ LCD_WIDTH ];
	for ( int i = 0; i < LCD_WIDTH; i++ )
		l_buf[ i ] = 0;

	for ( int i = 0; i < LCD_HEIGHT; i++ )
		lcd_write_data_buf( ( uint8_t * ) l_buf, sizeof( l_buf ) );
}

