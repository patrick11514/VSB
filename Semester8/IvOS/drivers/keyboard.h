#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

void keyboard_irq_handler(void);
int kernel_keyboard_getchar(void);

#endif // KEYBOARD_H
