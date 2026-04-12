#include "cli.h"
#include "drivers/ide.h"
#include "drivers/keyboard.h"
#include "drivers/serial.h"
#include "drivers/vga.h"
#include "fs_state.h"
#include "lib/string.h"

FatFileSystem g_fat_fs;
PartitionTable g_fat_partitions[4];

#define FAT16_VOLUME_START_SECTOR 2048

static int mount_fat16_partition(void) {
  fat_init(&g_fat_fs);

  memset(&g_fat_partitions[0], 0, sizeof(PartitionTable));
  g_fat_partitions[0].partition_type = 0x06;
  g_fat_partitions[0].start_sector = FAT16_VOLUME_START_SECTOR;

  fat_select_partition_table(&g_fat_fs, &g_fat_partitions[0]);
  fat_read_boot_sector(&g_fat_fs);

  vga_print("Mounted FAT16 volume at sector 2048.\n");
  serial_print("Mounted FAT16 volume at sector 2048.\n");
  return 1;
}

void kernel_main() {
  // Initialize serial first for debug output
  serial_init();
  serial_print("Serial port initialized!\n");

  // Clear screen and set some cool default colors
  vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
  vga_clear_screen();

  // Print welcome message
  vga_print_centered("   *     \n");
  vga_print_centered("     .  *\n");
  vga_print_centered(" * .     \n");
  vga_print_centered("   * .  *\n");
  vga_print_centered("*        \n");
  vga_putchar('\n');

  vga_set_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
  vga_print_centered("PleiadOS\n");
  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  vga_print_centered("Welcome...\n\n");

  mount_fat16_partition();

  serial_print("PleiadOS booted successfully!\n");

  cli_loop();
}
