#include "cli.h"
#include "drivers/ide.h"
#include "drivers/keyboard.h"
#include "drivers/serial.h"
#include "drivers/vga.h"
#include "lib/string.h"

static uint32_t parse_hex(const char *str) {
  uint32_t val = 0;
  if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
    str += 2;
  while (*str) {
    char c = *str;
    val <<= 4;
    if (c >= '0' && c <= '9')
      val |= (c - '0');
    else if (c >= 'a' && c <= 'f')
      val |= (c - 'a' + 10);
    else if (c >= 'A' && c <= 'F')
      val |= (c - 'A' + 10);
    else
      return val >> 4;
    str++;
  }
  return val;
}

static uint32_t parse_dec(const char *str) {
  uint32_t val = 0;
  while (*str) {
    char c = *str;
    if (c >= '0' && c <= '9') {
      val = val * 10 + (c - '0');
    } else {
      return val;
    }
    str++;
  }
  return val;
}

static void print_hex(uint32_t val, int width) {
  char buf[16];
  int pos = 0;
  if (val == 0) {
    while (pos < width)
      buf[pos++] = '0';
  } else {
    while (val > 0) {
      int r = val & 0xF;
      buf[pos++] = (r < 10) ? ('0' + r) : ('A' + r - 10);
      val >>= 4;
    }
    while (pos < width)
      buf[pos++] = '0';
  }
  for (int i = pos - 1; i >= 0; i--) {
    vga_putchar(buf[i]);
  }
}

static void print_dec(uint32_t val) {
  if (val == 0) {
    vga_putchar('0');
    return;
  }
  char buf[16];
  int pos = 0;
  while (val > 0) {
    buf[pos++] = '0' + (val % 10);
    val /= 10;
  }
  for (int i = pos - 1; i >= 0; i--) {
    vga_putchar(buf[i]);
  }
}

// --- App header for dynamic run ---
struct AppHeader {
  uint32_t magic;
  char name[16];
  uint32_t start_lba;
  uint32_t load_addr;
  uint32_t size_sectors;
} __attribute__((packed));

#define APP_MAGIC 0xDEADBEEF
static const uint32_t app_slots[] = {100, 110, 120}; // Slots at LBAs
#define NUM_SLOTS (sizeof(app_slots) / sizeof(app_slots[0]))

// --- CLI State ---
static uint8_t disk_buffer[512];

#define LINE_MAX 128
#define ARGS_MAX 10

static void cli_readline(char *buffer, int max_len) {
  int i = 0;
  while (1) {
    char c = keyboard_getchar();
    if (c == '\n' || c == '\r') {
      vga_putchar('\n');
      buffer[i] = '\0';
      break;
    } else if (c == '\b' || c == 8) { // Backspace
      if (i > 0) {
        i--;
        vga_putchar('\b');
      }
    } else if (c >= 32 && c <= 126) {
      if (i < max_len - 1) {
        buffer[i++] = c;
        vga_putchar(c);
      }
    }
  }
}

static int cli_parse(char *line, char *argv[], int max_args) {
  int argc = 0;
  char *ptr = line;
  while (*ptr && argc < max_args) {
    while (*ptr == ' ')
      ptr++;
    if (*ptr == '\0')
      break;
    argv[argc++] = ptr;
    while (*ptr != ' ' && *ptr != '\0')
      ptr++;
    if (*ptr == ' ') {
      *ptr = '\0';
      ptr++;
    }
  }
  return argc;
}

static void do_help() {
  vga_print("Available commands:\n");
  vga_print("  help                 - Show this help\n");
  vga_print("  clear                - Clear the screen\n");
  vga_print("  read <lba>           - Read sector into internal buffer\n");
  vga_print("  write <lba>          - Write internal buffer to sector\n");
  vga_print("  dump <addr>          - Hex dump 128 bytes from addr\n");
  vga_print("  load <lba> <addr>    - Load 1 sector from lba to addr\n");
  vga_print("  run <addr>           - Jump to addr\n");
  vga_print("  list                 - List available programs in slots\n");
  vga_print("  exec <name>          - Load and run a program by name\n");
}

static void do_dump(uint32_t addr) {
  uint8_t *p = (uint8_t *)addr;
  for (int i = 0; i < 128; i += 16) {
    print_hex(addr + i, 8);
    vga_print("  ");
    for (int j = 0; j < 16; j++) {
      print_hex(p[i + j], 2);
      vga_putchar(' ');
    }
    vga_print(" |");
    for (int j = 0; j < 16; j++) {
      char c = p[i + j];
      if (c >= 32 && c <= 126)
        vga_putchar(c);
      else
        vga_putchar('.');
    }
    vga_print("|\n");
  }
}

static void do_list() {
  vga_print("Scanning slots...\n");
  for (int i = 0; i < NUM_SLOTS; i++) {
    uint32_t lba = app_slots[i];
    if (ide_read_sector(lba, disk_buffer) == 0) {
      struct AppHeader *hdr = (struct AppHeader *)disk_buffer;
      if (hdr->magic == APP_MAGIC) {
        vga_print("Slot ");
        print_dec(i);
        vga_print(" (LBA ");
        print_dec(lba);
        vga_print("): ");

        // Copy name safely to print
        char name[17];
        for (int k = 0; k < 16; k++)
          name[k] = hdr->name[k];
        name[16] = 0;

        vga_print(name);
        vga_print(" [Load Addr: 0x");
        print_hex(hdr->load_addr, 8);
        vga_print(" Size: ");
        print_dec(hdr->size_sectors);
        vga_print(" sectors]\n");
      }
    }
  }
}

static void do_exec(const char *target_name) {
  for (int i = 0; i < NUM_SLOTS; i++) {
    uint32_t lba = app_slots[i];
    if (ide_read_sector(lba, disk_buffer) == 0) {
      struct AppHeader *hdr = (struct AppHeader *)disk_buffer;
      if (hdr->magic == APP_MAGIC) {
        char name[17];
        for (int k = 0; k < 16; k++)
          name[k] = hdr->name[k];
        name[16] = 0;

        if (strcmp(name, target_name) == 0) {
          vga_print("Loading '");
          vga_print(name);
          vga_print("'...\n");

          uint8_t *ptr = (uint8_t *)hdr->load_addr;
          uint32_t current_lba = hdr->start_lba;

          for (uint32_t s = 0; s < hdr->size_sectors; s++) {
            if (ide_read_sector(current_lba, ptr) != 0) {
              vga_print("Error reading sector ");
              print_dec(current_lba);
              vga_print("\n");
              return;
            }
            ptr += 512;
            current_lba++;
          }

          vga_print("Running '");
          vga_print(name);
          vga_print("'...\n");

          void (*func)() = (void (*)())hdr->load_addr;
          func();

          return;
        }
      }
    }
  }
  vga_print("Program not found: ");
  vga_print(target_name);
  vga_print("\n");
}

void cli_loop() {
  char line[LINE_MAX];
  char *argv[ARGS_MAX];

  vga_print("\nType 'help' for a list of commands.\n");

  while (1) {
    vga_print("> ");
    cli_readline(line, LINE_MAX);

    if (strlen(line) == 0)
      continue;

    int argc = cli_parse(line, argv, ARGS_MAX);
    if (argc == 0)
      continue;

    if (strcmp(argv[0], "help") == 0) {
      do_help();
    } else if (strcmp(argv[0], "clear") == 0) {
      vga_clear_screen();
    } else if (strcmp(argv[0], "read") == 0) {
      if (argc >= 2) {
        uint32_t lba = parse_dec(argv[1]);
        if (ide_read_sector(lba, disk_buffer) == 0) {
          vga_print("Read LBA ");
          print_dec(lba);
          vga_print(" into buffer.\n");
        } else {
          vga_print("Disk read error.\n");
        }
      } else {
        vga_print("Usage: read <lba>\n");
      }
    } else if (strcmp(argv[0], "write") == 0) {
      if (argc >= 2) {
        uint32_t lba = parse_dec(argv[1]);
        if (ide_write_sector(lba, disk_buffer) == 0) {
          vga_print("Wrote buffer to LBA ");
          print_dec(lba);
          vga_print(".\n");
        } else {
          vga_print("Disk write error.\n");
        }
      } else {
        vga_print("Usage: write <lba>\n");
      }
    } else if (strcmp(argv[0], "dump") == 0) {
      if (argc >= 2) {
        uint32_t addr = parse_hex(argv[1]);
        do_dump(addr);
      } else {
        vga_print("Usage: dump <addr>\n");
      }
    } else if (strcmp(argv[0], "load") == 0) {
      if (argc >= 3) {
        uint32_t lba = parse_dec(argv[1]);
        uint32_t addr = parse_hex(argv[2]);
        if (ide_read_sector(lba, (void *)addr) == 0) {
          vga_print("Loaded LBA ");
          print_dec(lba);
          vga_print(" to 0x");
          print_hex(addr, 8);
          vga_print("\n");
        } else {
          vga_print("Disk read error.\n");
        }
      } else {
        vga_print("Usage: load <lba> <addr>\n");
      }
    } else if (strcmp(argv[0], "run") == 0) {
      if (argc >= 2) {
        if (strcmp(argv[1], "hello") == 0) {
          vga_print("Did you mean 'exec hello'? 'run' is only for raw hex "
                    "addresses (e.g., run 2000).\n");
          continue;
        }
        uint32_t addr = parse_hex(argv[1]);
        if (addr == 0 && argv[1][0] != '0') {
          vga_print("Invalid hex address.\n");
          continue;
        }
        vga_print("Jumping to 0x");
        print_hex(addr, 8);
        vga_print("...\n");
        void (*func)() = (void (*)())addr;
        func();
      } else {
        vga_print("Usage: run <addr>\n");
      }
    } else if (strcmp(argv[0], "list") == 0) {
      do_list();
    } else if (strcmp(argv[0], "exec") == 0) {
      if (argc >= 2) {
        do_exec(argv[1]);
      } else {
        vga_print("Usage: exec <name>\n");
      }
    } else {
      vga_print("Unknown command: ");
      vga_print(argv[0]);
      vga_print("\n");
    }
  }
}
