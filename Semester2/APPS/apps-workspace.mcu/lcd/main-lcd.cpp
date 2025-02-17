// **************************************************************************
//
//               Demo program for APPS labs
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 02/2022
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         Main program for LCD module
//
// **************************************************************************

#include "mbed.h"
#include "lcd_lib.h"

// two dimensional array with fixed size font
extern uint8_t g_font8x8[256][8];

DigitalOut g_led_PTA1( PTA1, 0 );
DigitalOut g_led_PTA2( PTA2, 0 );

DigitalIn g_but_PTC9( PTC9 );
DigitalIn g_but_PTC10( PTC10 );
DigitalIn g_but_PTC11( PTC11 );
DigitalIn g_but_PTC12( PTC12 );

int main()
{
	lcd_init();				// LCD initialization

	uint16_t l_color_red = 0xF800;
	uint16_t l_color_green = 0x07E0;
	uint16_t l_color_blue = 0x001F;
	uint16_t l_color_white = 0xFFFF;

	// simple animation display four color square using LCD_put_pixel function
	int32_t l_limit = 200;

	for ( int32_t ofs = 0; ofs < 20; ofs++ ) // square offset in x and y axis
		for ( int32_t i = 0; i < l_limit; i++ )
		{
			lcd_put_pixel( ofs + i,       ofs + 0,       l_color_red );
			lcd_put_pixel( ofs + 0,       ofs + i,       l_color_green );
			lcd_put_pixel( ofs + i,       ofs + l_limit, l_color_blue );
			lcd_put_pixel( ofs + l_limit, ofs + i,       l_color_white );
		}

	return 0;
}
