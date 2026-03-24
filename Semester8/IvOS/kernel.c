#include "drivers/ide.h"
#include "drivers/keyboard.h"
#include "drivers/serial.h"
#include "drivers/vga.h"

void kernel_main() {
  // Initialize serial first for debug output
  serial_init();
  serial_print("Serial port initialized!\n");

  // Clear screen and set some cool default colors
  vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
  vga_clear_screen();

  // Print welcome message
  vga_print_centered("   *\n");
  vga_print_centered("     .  *\n");
  vga_print_centered(" * .\n");
  vga_print_centered("   * .  *\n");
  vga_print_centered("*\n");
  vga_putchar('\n');

  vga_set_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
  vga_print_centered("PleiadOS\n");
  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  vga_print_centered("Welcome...\n\n");

  serial_print("PleiadOS booted successfully!\n");

  // Simple keyboard echo loop to test
  vga_print("Type something (basic ASCII): ");
  while (1) {
    char c = keyboard_getchar();
    if (c) {
      vga_putchar(c);
      serial_putchar(c);
    }
  }
}
