#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

int keyboard_getchar(void);
int keyboard_handle_scancode_irq(uint8_t scancode);

#endif // KEYBOARD_H
