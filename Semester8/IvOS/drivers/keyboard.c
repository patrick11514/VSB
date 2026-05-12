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

#define KBD_BUF_SIZE 256
static char kbd_buf[KBD_BUF_SIZE];
static volatile int head = 0;
static volatile int tail = 0;

#define KBD_SCANCODE_BUF_SIZE 256
static uint8_t scancode_buf[KBD_SCANCODE_BUF_SIZE];
static volatile int scancode_head = 0;
static volatile int scancode_tail = 0;

int keyboard_handle_scancode_irq(uint8_t scancode) {
  /* Update modifier state */
  if (scancode == 0x38) {
    alt_held = 1;
    return 0;
  } /* Alt Pressed */
  if (scancode == 0xB8) {
    alt_held = 0;
    return 0;
  } /* Alt Released */

  if (scancode == 0x2A || scancode == 0x36) {
    shift_held = 1;
    return 0;
  }
  if (scancode == 0xAA || scancode == 0xB6) {
    shift_held = 0;
    return 0;
  }

  if (scancode == 0x1D) {
    ctrl_held = 1;
    return 0;
  }
  if (scancode == 0x9D) {
    ctrl_held = 0;
    return 0;
  }

  /* Check for Tab key (0x0F) */
  if (scancode == 0x0F) {
    if (alt_held) {
      /* We caught an ALT+TAB! Reset alt_held so it doesn't get stuck */
      alt_held = 0;
      return 1; /* Return 1 to tell the IRQ handler to switch tasks */
    }
  }

  return 0; /* Normal key, do not intercept */
}

void keyboard_irq_handler(void) {
  uint8_t scancode = inb(KEYBOARD_DATA_PORT);

  if (keyboard_handle_scancode_irq(scancode) == 1) {
    extern void cli_handle_alt_tab(void);
    cli_handle_alt_tab();
    return; /* DO NOT put ALT+TAB into the buffer! */
  }

  int next_scancode_head = (scancode_head + 1) % KBD_SCANCODE_BUF_SIZE;
  if (next_scancode_head != scancode_tail) {
    scancode_buf[scancode_head] = scancode;
    scancode_head = next_scancode_head;
  }

  /* 1. Ignore key releases (top bit is set) */
  if (scancode & 0x80) {
    return;
  }

  /* 2. Prevent array out of bounds */
  if (scancode < sizeof(scancode_to_ascii)) {
    char c = scancode_to_ascii[scancode];
    if (c) {
      int next_head = (head + 1) % KBD_BUF_SIZE;
      if (next_head != tail) { /* Prevent overflow */
        kbd_buf[head] = c;
        head = next_head;
      }
    }
  }
}

int kernel_keyboard_getchar(void) {
  /* Read from the translated ASCII buffer! */
  while (head == tail) {
    extern void scheduler_yield(void);
    scheduler_yield();
  }

  char c = kbd_buf[tail];
  tail = (tail + 1) % KBD_BUF_SIZE;
  return c;
}

int kernel_keyboard_poll_scancode(void) {
  /* Read from the raw hardware scancode buffer! */
  if (scancode_head == scancode_tail)
    return 0;

  uint8_t scancode = scancode_buf[scancode_tail];
  scancode_tail = (scancode_tail + 1) % KBD_SCANCODE_BUF_SIZE;
  return (int)scancode;
}