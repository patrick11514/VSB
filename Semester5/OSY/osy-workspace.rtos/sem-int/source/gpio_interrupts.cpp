// **************************************************************************
//
//               FreeRTOS demo program for OSY labs
//
// Subject:      Operating Systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 02/2022
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         GPIO initialization for interrupts
//
// **************************************************************************
//
// FreeRTOS kernel includes.
#include "FreeRTOS.h"
#include "semphr.h"

// System includes.
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
//#include "clock_config.h"
//#include "MK64F12.h"
//#include "fsl_debug_console.h"
#include "gpio_interrupts.h"


extern "C" {
void PORTC_IRQHandler(void);
}

// ISR for PORTC
void PORTC_IRQHandler(void)
{
	// DO NOT USE return in this Interrupt Service Routine!!!

    // Get mask of all current interrupts
	uint32_t l_mask = GPIO_PortGetInterruptFlags( SW_PTC9_GPIO );

	TickType_t l_cur_tick = xTaskGetTickCount();

	// Switch PTC9
	if ( l_mask & SW_PTC9_GPIO_PIN_MASK )
	{
		// RECOMMENDED SOLUTION!!!
		// wakeup some process using semaphore
		xSemaphoreGiveFromISR( g_sem_left_switch, nullptr );
	}

	// Switch PTC10
	if ( l_mask & SW_PTC10_GPIO_PIN_MASK )
	{
	}

	// Switch PTC11
	if ( l_mask & SW_PTC11_GPIO_PIN_MASK )
	{
	}

	// Switch PTC12
	if ( l_mask & SW_PTC12_GPIO_PIN_MASK )
	{
		// INCORRECT SOLUTION!!!
		// Direct changing global variable will not send any event to RTOS application
		if ( g_red_led_index < 7 )
			g_red_led_index++;

		// Correct solution: use semaphore. See also Switch PTC9 above.
		// xSemaphoreGiveFromISR( g_sem_right_switch, nullptr );
	}

	// Clear interrupt flags
    GPIO_PortClearInterruptFlags( SW_PTC9_GPIO,
    		SW_PTC9_GPIO_PIN_MASK | SW_PTC10_GPIO_PIN_MASK | SW_PTC11_PIN_MASK | SW_PTC12_PIN_MASK );
}

// Initialize GPIO for interrupts
void InitGPIOInterrupts()
{
	// Set correct priority
    NVIC_SetPriority( PORTC_IRQn, 3 );

    // Enable interrupt for switches
    // Available interrupt methods: kPORT_InterruptFallingEdge,kPORT_InterruptRisingEdge, kPORT_InterruptEitherEdge;
    PORT_SetPinInterruptConfig( SW_PTC9_PORT, SW_PTC9_PIN, kPORT_InterruptFallingEdge );
    PORT_SetPinInterruptConfig( SW_PTC9_PORT, SW_PTC10_PIN, kPORT_InterruptFallingEdge );
    PORT_SetPinInterruptConfig( SW_PTC9_PORT, SW_PTC11_PIN, kPORT_InterruptFallingEdge );
    PORT_SetPinInterruptConfig( SW_PTC9_PORT, SW_PTC12_PIN, kPORT_InterruptFallingEdge );

    // Enable interrupt
    EnableIRQ( PORTC_IRQn );

    // Create binary semaphores
    g_sem_left_switch = xSemaphoreCreateBinary();
    g_sem_right_switch = xSemaphoreCreateBinary();
}
