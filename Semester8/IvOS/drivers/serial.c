#include "serial.h"
#include "../arch/io.h"

#define COM1 0x3F8

void serial_init() {
  outb(COM1 + 1, 0x00); // Disable all interrupts
  outb(COM1 + 3, 0x80); // Enable DLAB (set baud rate divisor)
  outb(COM1 + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
  outb(COM1 + 1, 0x00); //                  (hi byte)
  outb(COM1 + 3, 0x03); // 8 bits, no parity, one stop bit
  outb(COM1 + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
  outb(COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

static int serial_is_transmit_empty() { return inb(COM1 + 5) & 0x20; }

void serial_putchar(char c) {
  while (serial_is_transmit_empty() == 0)
    ;
  outb(COM1, c);
}

void serial_print(const char *str) {
  while (*str) {
    serial_putchar(*str++);
  }
}
