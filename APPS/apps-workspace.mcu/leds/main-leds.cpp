// **************************************************************************
//
//               Demo program for APPS labs
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 02/2022
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         Main program for LEDs
//
// **************************************************************************

#include "mbed.h"


/*
// LEDs on K64F-KIT - instances of class DigitalOut
DigitalOut g_led_PTA1( PTA1 );
DigitalOut g_led_PTA2( PTA2 );
DigitalOut g_led_PTC0(PTC0);
DigitalOut g_led_PTC1(PTC1);
DigitalOut g_led_PTC2(PTC2);
DigitalOut g_led_PTC3(PTC3);
DigitalOut g_led_PTC4(PTC4);
DigitalOut g_led_PTC5(PTC5);
DigitalOut g_led_PTC7(PTC7);
DigitalOut g_led_PTC8(PTC8);

// Button on K64F-KIT - instance of class DigitalIn
DigitalIn g_but_PTC9( PTC9 );
DigitalIn g_but_PTC10( PTC10 );
DigitalIn g_but_PTC11( PTC11 );
DigitalIn g_but_PTC12( PTC12 );

void control_from_ticker()
{
	static int l_ticks = 0;

	int32_t l_periode = 500;		// T/2 = 0.5 sec

	if ( g_but_PTC9 == 0 )			// check button
	{
		l_periode /= 10;			// increase speed of blinking
	}

	if ( l_ticks >= l_periode / 2 )	// time to switch on?
	{
		g_led_PTA2 = 1;				// LED On
	}

	l_ticks++;						// milliseconds counter

	if ( l_ticks >= l_periode )		// end of periode?
	{
		l_ticks = 0;				// start periode again

		g_led_PTA2 = 0;				// LED Off
	}
}


void test1() {
	static int tick = 0;
	static int percentage = 100;
	static bool firstPressed = false;
	static bool secondPressed = false;

	int32_t l_periode = 20;

	printf("%d\n", percentage);
	printf("%d %d\n", firstPressed, secondPressed);

	if (g_but_PTC9 == 0 && !firstPressed) {
		if (percentage != 0) {
			firstPressed = true;
			percentage -= 10;
		}
	}

	if (g_but_PTC9 == 1){
		firstPressed = false;
	}

	if (g_but_PTC10 == 0 && !secondPressed) {
		if (percentage != 100) {
			secondPressed = true;
			percentage += 10;
		}
	}

	if (g_but_PTC10 == 1){
		secondPressed = false;
	}

	if (tick >= ((l_periode * percentage) / 100)) {
		g_led_PTA1 = 0;
	}

	tick++;

	if (tick >= l_periode) {
		tick = 0;
		if (percentage > 0)
			g_led_PTA1 = 1;
	}
}

void changeLed(int index, bool status) {
	switch (index) {
	case 0:
		g_led_PTC0 = status;
		break;
	case 1:
		g_led_PTC1 = status;
		break;
	case 2:
		g_led_PTC2 = status;
		break;
	case 3:
		g_led_PTC3 = status;
		break;
	case 4:
		g_led_PTC4 = status;
		break;
	case 5:
		g_led_PTC5 = status;
		break;
	case 6:
		g_led_PTC7 = status;
		break;
	case 7:
		g_led_PTC8 = status;
		break;
	}
}

void sus() {
	static int tick = 0;
	static int t_switch = 0;
	static int percentage = 100;
	static bool firstPressed = false;
	static bool secondPressed = false;
	static bool thirdPressed = false;
	static bool fourthPressed = false;
	static bool reverse = false;
	static int index = 0;

	int32_t l_periode = 20;
	int32_t l_switch = 100;

	printf("%d\n", percentage);
	printf("%d", index);

	if (g_but_PTC9 == 0 && !firstPressed) {
		if (percentage != 0) {
			firstPressed = true;
			percentage -= 10;
		}
	}

	if (g_but_PTC9 == 1){
		firstPressed = false;
	}

	if (g_but_PTC10 == 0 && !secondPressed) {
		if (percentage != 100) {
			secondPressed = true;
			percentage += 10;
		}
	}

	if (g_but_PTC10 == 1){
		secondPressed = false;
	}



	if (tick >= ((l_periode * percentage) / 100)) {
		changeLed(index, false);
	}

	tick++;
	t_switch++;

	if (tick >= l_periode) {
		tick = 0;
		if (percentage > 0)
			changeLed(index, true);
	}

	if (t_switch >= l_switch) {
		t_switch = 0;
		if (!reverse)
			if (index == 7) {
				index = 0;
			} else {
				index++;
			}
		else
			if (index == 0) {
				index = 7;
			} else {
				index--;
			}
	}
}

void test2() {
	static int tick = 0;
	static int t_switch = 0;
	static int percentage = 100;
	static bool firstPressed = false;
	static bool secondPressed = false;
	static bool thirdPressed = false;
	static bool fourthPressed = false;
	static bool reverse = false;
	static int index = 0;

	int32_t l_periode = 20;
	int32_t l_switch = 1500;

	printf("%d\n", percentage);
	printf("%d", index);

	if (g_but_PTC9 == 0 && !firstPressed) {
		if (percentage != 0) {
			firstPressed = true;
			percentage -= 10;
		}
	}

	if (g_but_PTC9 == 1){
		firstPressed = false;
	}

	if (g_but_PTC10 == 0 && !secondPressed) {
		if (percentage != 100) {
			secondPressed = true;
			percentage += 10;
		}
	}

	if (g_but_PTC10 == 1){
		secondPressed = false;
	}



	if (tick >= ((l_periode * percentage) / 100)) {
		changeLed(index, false);
	}

	tick++;
	t_switch++;

	if (tick >= l_periode) {
		tick = 0;
		if (percentage > 0) {
			changeLed(index, true);
		}
	}

	if (t_switch >= l_switch) {
		t_switch = 0;
		changeLed(index, false);
		if (!reverse)
			if (index == 7) {
				index = 0;
			} else {
				index++;
			}
		else
			if (index == 0) {
				index = 7;
			} else {
				index--;
			}
	}
}
*/
#define T 20

class PWMLed
{
private:
    Ticker ticker;
    DigitalOut led;
    uint8_t brightness_percent;
    uint32_t on_period;
    uint32_t ticks;


public:
    PWMLed(PinName pin_name) : led(pin_name)
    {
    	std::chrono::milliseconds l_tout( 1 );

        this->ticker.attach(callback(this, &PWMLed::run_pwm), l_tout);
    }

    void set_brightness(uint8_t new_percent)
    {
    	if (new_percent <= 100) {

    		this->brightness_percent = new_percent;

    		this->on_period = (T * this->brightness_percent) / 100;
    	}
    }

    uint8_t get_brightness() {
    	return this->brightness_percent;
    }

    void run_pwm()
    {
        if (this->ticks < this->on_period) {
        	this->led = 1;
        } else {
        	this->led = 0;
        }

        this->ticks++;
        if (this->ticks >= T)
            this->ticks = 0;
    }
};


class RGBLed
{
private:
    Ticker ticker;
    DigitalOut r;
    DigitalOut g;
    DigitalOut b;
    uint8_t brightness_percent;
    uint32_t on_period;
    uint32_t ticks;

    uint8_t r_value;
    uint8_t g_value;
    uint8_t b_value;

public:
    RGBLed(PinName r, PinName g, PinName b) : r(r), g(g), b(b)
    {
    	std::chrono::milliseconds l_tout( 1 );

        this->ticker.attach(callback(this, &RGBLed::run_pwm), l_tout);
    }

    void set_brightness(uint8_t new_percent)
    {
    	if (new_percent <= 100) {

    		this->brightness_percent = new_percent;

    		this->on_period = (T * this->brightness_percent) / 100;
    	}
    }

    uint8_t get_brightness() {
    	return this->brightness_percent;
    }

    void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
    	this->r_value = r;
    	this->g_value = g;
    	this->b_value = b;
    }

    void run_pwm()
    {

        if (this->ticks < (((T * this->r_value) / 255) * this->brightness_percent) / 100) {
        	this->r = 1;
        } else {
        	this->r = 0;
        }

        if (this->ticks < (((T * this->g_value) / 255) * this->brightness_percent) / 100) {
			this->g = 1;
		} else {
			this->g = 0;
		}

        if (this->ticks < (((T * this->b_value) / 255) * this->brightness_percent) / 100) {
			this->b = 1;
		} else {
			this->b = 0;
		}

        this->ticks++;
        if (this->ticks >= T)
            this->ticks = 0;
    }
};


#define TB 50

class Button {
private:
	DigitalIn button;
	Ticker ticker;
	uint32_t ticks;
	bool status;
	bool last_status;

public:
	Button(PinName pin_name) : button(pin_name) {
		this->status = false;
		this->last_status = false;

		std::chrono::milliseconds l_tout( 1 );

		this->ticker.attach(callback(this, &Button::run_pwm), l_tout);
	}

	void run_pwm() {
		this->ticks++;
		if (this->ticks >= TB) {
		   this->ticks = 0;
		   if (this->button == 0) {
			   this->status = true;
		   } else {
			   this->status = false;
		   }
		}
	}

	bool get_status() {
		return this->status;
	}

	bool get_change() {
		if (this->last_status != this->status) {
			this->last_status = this->status;
			return this->status;
		}
		return false;
	}
};

PWMLed leds[] = {
    { PTC0 },
    { PTC1 },
	{ PTC2 },
	{ PTC3 },
	{ PTC4 },
	{ PTC5 },
	{ PTC7 },
	{ PTC8 },
	{ PTA1 },
	{ PTA2 }
};

Button buttons[] = {
	{ PTC9  },
	{ PTC10 },
	{ PTC11 },
	{ PTC12 }
};

RGBLed rgbLeds[] = {
		{PTB2, PTB3, PTB9},
		{PTB10, PTB11, PTB18},
		{PTB19, PTB20, PTB23},
};

//LEDS2
void blink() {
	static int timer = 0;

	for (int i = 0; i < 10; i++) {
		if (i <= timer) {
			int bright = leds[i].get_brightness();
			leds[i].set_brightness(bright == 100 ? 0 : 100);
		}
	}

	timer++;

	if (timer >= 10) {
		timer = 0;
	}
}

//PREPARATION
void checkButtons() {
	static int selected = 0;

	if (buttons[0].get_change()) {
		selected--;
		if (selected < 0) {
			selected = 9;
		}
	}

	if (buttons[1].get_change()) {
		selected++;
		if (selected >= 10) {
			selected = 0;
		}
	}

	if (buttons[2].get_status()) {
		int bright = leds[selected].get_brightness() - 10;
		if (bright < 0) {
			bright = 0;
		}

		leds[selected].set_brightness(bright);
	}

	if (buttons[3].get_status()) {
		int bright = leds[selected].get_brightness() + 10;
		if (bright > 100) {
			bright = 100;
		}

		leds[selected].set_brightness(bright);
	}

}

void change_brightness() {
	static int b1 = 100;
	static int b2 = 50;
	static int b3 = 0;

	b1 += 5;
	b2 += 5;
	b3 += 5;

	if (b1 >= 100) b1 = 0;
	if (b2 >= 100) b2 = 0;
	if (b3 >= 100) b3 = 0;

	rgbLeds[0].set_brightness(b1);
	rgbLeds[1].set_brightness(b2);
	rgbLeds[2].set_brightness(b3);
}

//LEDS2
void randomLed() {
	static int turned = -1;
	static int selected = 0;

	if(buttons[0].get_status()) {
		turned = selected;
		leds[turned].set_brightness(100);
	} else {
		leds[turned].set_brightness(0);
		turned = -1;
	}

	if (turned == -1)
		selected++;

	if (selected > 7) {
		selected = 0;
	}
}

void knightRider() {
	static bool running = false;
	static int timer = 0;
	static int current = 0;
	static bool reverse = false;

	int maxTimer = 500;

	if (buttons[0].get_change()) {
		//if turning off, turn off current led
		leds[current].set_brightness(0);

		//reverse
		running = !running;
		//set current to -1, because if we want to start with led 0
		current = -1;
		timer = maxTimer;
		//reset reverse
		reverse = false;
	}


	timer++;

	if (timer >= maxTimer) {
		timer = 0;

		if (running) {
			leds[current].set_brightness(0);

			if (reverse){
				current--;
				if (current <= 0) {
					reverse = !reverse;
				}
			} else {
				current++;
				if (current >= 7) {
					reverse = !reverse;
				}
			}

			leds[current].set_brightness(100);
		}
	}
}

void blinker() {
	static int timer = 0;
	static bool status = false;
	static bool runningLeft = false;
	static bool runningRight = false;

	int max = 750;

	if (buttons[0].get_change()) {
		runningLeft = !runningLeft;

		rgbLeds[0].set_brightness(0);
	}

	if (buttons[1].get_change()) {
		runningRight = !runningRight;

		rgbLeds[2].set_brightness(0);
	}

	timer++;

	if (timer >= max) {
		timer = 0;

		if (runningLeft || runningRight) {
			status = !status;
		}

		if (runningLeft) {
			if (status) {
				rgbLeds[0].set_brightness(100);
			} else {
				rgbLeds[0].set_brightness(0);
			}
		}

		if (runningRight) {
			if (status) {
				rgbLeds[2].set_brightness(100);
			} else {
				rgbLeds[2].set_brightness(0);
			}
		}
	}
}

int main()
{
	printf( "LED demo program started...\n" );
	Ticker ticker;

	std::chrono::milliseconds time(1);

	rgbLeds[0].set_rgb(255, 100, 0);
	rgbLeds[2].set_rgb(255, 100, 0);

	ticker.attach(knightRider, time);


	while ( 1 )
	{ // infinite loop

	}

}
