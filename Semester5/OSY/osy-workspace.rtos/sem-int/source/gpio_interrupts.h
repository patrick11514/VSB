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

// Semaphores for synchronization ISR and application
extern SemaphoreHandle_t g_sem_left_switch;
extern SemaphoreHandle_t g_sem_right_switch;

// Position of active red LED
extern int32_t g_red_led_index;

// Initialize GPIO for interrupts
void InitGPIOInterrupts();


