#include "keyboard.h"
#include "../arch/io.h"

// Scancode to ASCII map for a standard US keyboard
// Only mapping basic keys based on the prompt
static const char scancode_to_ascii[] = {
    0,    27,  '1',  '2', '3',  '4', '5',  '6',
    '7',  '8', '9',  '0', '-',  '=', '\b', '\t', /* Tab */
    'q',  'w', 'e',  'r', 't',  'y', 'u',  'i',
    'o',  'p', '[',  ']', '\n', 0, /* Control */
    'a',  's', 'd',  'f', 'g',  'h', 'j',  'k',
    'l',  ';', '\'', '`', 0, /* Left shift */
    '\\', 'z', 'x',  'c', 'v',  'b', 'n',  'm',
    ',',  '.', '/',  0, /* Right shift */
    '*',  0,            /* Alt */
    ' ',                /* Space bar */
};

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

int keyboard_getchar() {
  while (1) {
    // Wait until keyboard buffer has data
    if (inb(KEYBOARD_STATUS_PORT) & 1) {
      uint8_t scancode = inb(KEYBOARD_DATA_PORT);

      // If the highest bit is set, it's a key release -> ignore
      if (scancode & 0x80)
        continue;

      // Check bounds
      if (scancode < sizeof(scancode_to_ascii)) {
        char c = scancode_to_ascii[scancode];
        if (c) {
          return c;
        }
      }
    }
  }
}
