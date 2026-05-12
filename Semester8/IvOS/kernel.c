#include "arch/interrupts.h"
#include "cli.h"
#include "drivers/ide.h"
#include "drivers/keyboard.h"
#include "drivers/serial.h"
#include "drivers/vga.h"
#include "fs_state.h"
#include "kernel/scheduler.h"
#include "lib/convert.h"
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

void counter(void *arg) {
  int count = 0;
  int sub_count = 0;
  char buf[16];

  while (1) {
    if (++sub_count >= 10000000) {
      sub_count = 0;
      serial_print("Counter: ");
      itoc(++count, buf);
      serial_print(buf);
      serial_print("\n");
    }
  }
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

  /* initialize scheduler */
  extern void scheduler_init(void);
  scheduler_init();

  /* interrupts: IDT, PIC, PIT */
  pic_remap();
  /* set timer ISR gate (IRQ0 -> vector 0x20) */
  idt_set_gate(0x20, (uint32_t)isr_timer_stub);
  /* set keyboard ISR gate (IRQ1 -> vector 0x21) */
  idt_set_gate(0x21, (uint32_t)isr_keyboard_stub);

  idt_load();
  timer_init(100); /* 100 Hz */
  /* enable interrupts */
  __asm__ volatile("sti");

  /* Create CLI as a scheduled thread (tid will be 0, pid will be 1) */
  extern void cli_thread_main(void *arg);
  extern int scheduler_create(void (*entry)(void *), void *arg, int priority);
  extern int scheduler_activate(int tid, void (*entry)(void *));
  extern void scheduler_set_name(int tid, const char *name);

  // simple counter
  int counter_tid = scheduler_create(counter, NULL, 0);
  if (counter_tid >= 0) {
    scheduler_activate(counter_tid, counter);
    scheduler_set_name(counter_tid, "counter");
  }

  int cli_tid = scheduler_create(cli_thread_main, NULL, 0);
  if (cli_tid >= 0) {
    scheduler_activate(cli_tid, cli_thread_main);
    scheduler_set_name(cli_tid, "cli");
  }

  /* Run the scheduler: timer IRQs will drive scheduling. Just loop. */
  while (1) {
    __asm__ volatile("hlt"); /* Wait for interrupt */
  }
}
