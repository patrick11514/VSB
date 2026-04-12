#include "vga.h"
#include "../arch/io.h"
#include "../lib/types.h"

static int vga_x = 0;
static int vga_y = 0;
static uint8_t vga_color = VGA_COLOR_LIGHT_GREY | (VGA_COLOR_BLACK << 4);
static uint16_t *const vga_buffer = (uint16_t *)0xB8000;

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
  return (uint16_t)uc | (uint16_t)color << 8;
}

static void vga_update_cursor(int x, int y) {
  uint16_t pos = y * VGA_WIDTH + x;
  outb(0x3D4, 0x0F);
  outb(0x3D5, (uint8_t)(pos & 0xFF));
  outb(0x3D4, 0x0E);
  outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void vga_set_color(uint8_t fg, uint8_t bg) { vga_color = fg | (bg << 4); }

void vga_clear_screen() {
  for (int y = 0; y < VGA_HEIGHT; y++) {
    for (int x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      vga_buffer[index] = vga_entry(' ', vga_color);
    }
  }
  vga_x = 0;
  vga_y = 0;
  vga_update_cursor(vga_x, vga_y);
}

void vga_scroll() {
  for (int y = 0; y < VGA_HEIGHT - 1; y++) {
    for (int x = 0; x < VGA_WIDTH; x++) {
      vga_buffer[y * VGA_WIDTH + x] = vga_buffer[(y + 1) * VGA_WIDTH + x];
    }
  }
  for (int x = 0; x < VGA_WIDTH; x++) {
    vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', vga_color);
  }
  vga_y--;
}

void vga_putchar(char c) {
  if (c == '\n') {
    vga_x = 0;
    vga_y++;
  } else if (c == '\b' || c == 8) {
    if (vga_x > 0) {
      vga_x--;
    } else if (vga_y > 0) {
      vga_y--;
      vga_x = VGA_WIDTH - 1;
    }
    const size_t index = vga_y * VGA_WIDTH + vga_x;
    vga_buffer[index] = vga_entry(' ', vga_color);
  } else {
    const size_t index = vga_y * VGA_WIDTH + vga_x;
    vga_buffer[index] = vga_entry(c, vga_color);
    vga_x++;
    if (vga_x >= VGA_WIDTH) {
      vga_x = 0;
      vga_y++;
    }
  }

  if (vga_y >= VGA_HEIGHT) {
    vga_scroll();
  }

  vga_update_cursor(vga_x, vga_y);
}

void vga_print(const char *str) {
  for (size_t i = 0; str[i] != '\0'; i++) {
    vga_putchar(str[i]);
  }
}

void vga_print_centered(const char *str) {
  size_t len = 0;
  while (str[len] != '\0')
    len++;

  if (len < VGA_WIDTH) {
    int padding = (VGA_WIDTH - len) / 2;
    vga_x = padding; // Set cursor to padded offset
    vga_update_cursor(vga_x, vga_y);
  }
  vga_print(str);
}
