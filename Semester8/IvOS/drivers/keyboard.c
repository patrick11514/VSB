#include "keyboard.h"
#include "../arch/io.h"

// Scancode to ASCII map for a standard US keyboard
// Only mapping basic keys based on the prompt
static const char scancode_to_ascii[] = {
    0,    27,  '1',  '2', '3',  '4', '5',  '6',
    '7',  '8', '9',  '0', '-',  '=', '\b', '\t', /* Tab */
    'q',  'w', 'e',  'r', 't',  'z', 'u',  'i',
    'o',  'p', '[',  ']', '\n', 0, /* Control */
    'a',  's', 'd',  'f', 'g',  'h', 'j',  'k',
    'l',  ';', '\'', '`', 0, /* Left shift */
    '\\', 'y', 'x',  'c', 'v',  'b', 'n',  'm',
    ',',  '.', '/',  0, /* Right shift */
    '*',  0,            /* Alt */
    ' ',                /* Space bar */
};

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

static int ctrl_held = 0;
static int alt_held = 0;
static int shift_held = 0;

int keyboard_getchar() {
  extern void serial_print(const char *);
  extern void serial_putchar(char c);
  serial_print("[KBD_ENTER]");
  int loop_count = 0;
  while (1) {
    // Wait until keyboard buffer has data
    if (inb(KEYBOARD_STATUS_PORT) & 1) {
      serial_print("[KBD_HAS_DATA]");
      uint8_t scancode = inb(KEYBOARD_DATA_PORT);
      serial_print("[KBD:");
      serial_putchar('0' + (scancode >> 4));
      serial_putchar('0' + (scancode & 0xF));
      serial_print("] ");

      /* Alt key: press=0x38, release=0xB8 */
      if (scancode == 0x38) {
        alt_held = 1;
        continue;
      }
      if (scancode == 0xB8) {
        alt_held = 0;
        continue;
      }

      /* Shift keys */
      if (scancode == 0x2A || scancode == 0x36) {
        shift_held = 1;
        continue;
      }
      if (scancode == 0xAA || scancode == 0xB6) {
        shift_held = 0;
        continue;
      }

      if (scancode == 0x1D) {
        ctrl_held = 1;
        continue;
      }

      if (scancode == 0x9D) {
        ctrl_held = 0;
        continue;
      }

      /* Tab key: 0x0F. If Alt held, return special Alt+Tab code 0xFF */
      if (scancode == 0x0F) {
        if (alt_held) {
          alt_held = 0;
          serial_print("[ALT+TAB!]\n");
          return 0xFF; /* Special Alt+Tab indicator */
        }
        continue; /* Otherwise skip Tab */
      }

      // If the highest bit is set, it's a key release -> ignore
      if (scancode & 0x80)
        continue;

      // Check bounds
      if (scancode < sizeof(scancode_to_ascii)) {
        char c = scancode_to_ascii[scancode];
        if (c) {
          if (ctrl_held && c == 'd') {
            ctrl_held = 0;
            return 4;
          }
          ctrl_held = 0;
          return c;
        }
      }
    }
  }
}
