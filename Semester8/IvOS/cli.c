#include "cli.h"
#include "drivers/ide.h"
#include "drivers/keyboard.h"
#include "drivers/serial.h"
#include "drivers/vga.h"
#include "fs_state.h"
#include "lib/convert.h"
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

static void print_fat_name(const Fat16Entry *entry) {
  int has_ext = 0;

  for (int i = 0; i < 3; i++) {
    if (entry->ext[i] != ' ') {
      has_ext = 1;
      break;
    }
  }

  for (int i = 0; i < 8; i++) {
    if (entry->filename[i] == ' ') {
      continue;
    }
    vga_putchar(entry->filename[i]);
  }

  if (has_ext) {
    vga_putchar('.');
    for (int i = 0; i < 3; i++) {
      if (entry->ext[i] != ' ') {
        vga_putchar(entry->ext[i]);
      }
    }
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
static char cli_current_path[128] = "/";

#define LINE_MAX 128
#define ARGS_MAX 10

#define PATH_PART_MAX 16
#define PATH_PART_LEN 13

static char to_upper_ascii(char c) {
  if (c >= 'a' && c <= 'z')
    return (char)(c - ('a' - 'A'));
  return c;
}

static void print_padded_dec(uint32_t val, int width) {
  char buf[16];
  int pos = 0;

  do {
    buf[pos++] = (char)('0' + (val % 10));
    val /= 10;
  } while (val > 0);

  while (pos < width)
    buf[pos++] = '0';

  for (int i = pos - 1; i >= 0; i--) {
    vga_putchar(buf[i]);
  }
}

static void print_fat_datetime(uint16_t date, uint16_t time) {
  uint32_t year = (date >> 9) + 1980;
  uint32_t month = (date >> 5) & 0x0F;
  uint32_t day = date & 0x1F;
  uint32_t hour = (time >> 11) & 0x1F;
  uint32_t minute = (time >> 5) & 0x3F;
  uint32_t second = (time & 0x1F) * 2;

  print_padded_dec(year, 4);
  vga_putchar('-');
  print_padded_dec(month, 2);
  vga_putchar('-');
  print_padded_dec(day, 2);
  vga_putchar(' ');
  print_padded_dec(hour, 2);
  vga_putchar(':');
  print_padded_dec(minute, 2);
  vga_putchar(':');
  print_padded_dec(second, 2);
}

static int split_path_tokens(const char *path,
                             char parts[PATH_PART_MAX][PATH_PART_LEN],
                             int *out_count, int *out_absolute) {
  int count = 0;
  int i = 0;

  if (!path || !out_count || !out_absolute)
    return 0;

  *out_absolute = (path[0] == '/');

  while (path[i] != '\0') {
    while (path[i] == '/')
      i++;

    if (path[i] == '\0')
      break;

    if (count >= PATH_PART_MAX)
      return 0;

    int len = 0;
    while (path[i] != '\0' && path[i] != '/') {
      if (len >= PATH_PART_LEN - 1)
        return 0;
      parts[count][len++] = path[i++];
    }

    parts[count][len] = '\0';
    count++;
  }

  *out_count = count;
  return 1;
}

static int make_fat_83_name(const char *component, char out_name[8],
                            char out_ext[3]) {
  int i;
  int name_len = 0;
  int ext_len = 0;
  int seen_dot = 0;

  for (i = 0; i < 8; i++)
    out_name[i] = ' ';
  for (i = 0; i < 3; i++)
    out_ext[i] = ' ';

  if (!component || component[0] == '\0')
    return 0;

  for (i = 0; component[i] != '\0'; i++) {
    char c = component[i];
    if (c == '.') {
      if (seen_dot)
        return 0;
      seen_dot = 1;
      continue;
    }

    c = to_upper_ascii(c);
    if (!seen_dot) {
      if (name_len >= 8)
        return 0;
      out_name[name_len++] = c;
    } else {
      if (ext_len >= 3)
        return 0;
      out_ext[ext_len++] = c;
    }
  }

  if (name_len == 0)
    return 0;

  return 1;
}

static int find_entry_in_dir(FatFileSystem *fs, uint16_t dir_cluster,
                             const char *component, Fat16Entry *out_entry) {
  char target_name[8];
  char target_ext[3];
  int old_index = fs->current_file_index;
  uint32_t max_entries;

  if (!make_fat_83_name(component, target_name, target_ext))
    return 0;

  fs->current_file_index = -1;
  max_entries = (dir_cluster == 0) ? fs->boot_sector.root_dir_entries : 65535;

  for (uint32_t i = 0; i < max_entries; i++) {
    fat_read_directory_entry(fs, dir_cluster);
    Fat16Entry *entry = &fs->current_file;

    if (entry->filename[0] == 0x00)
      break;

    if ((unsigned char)entry->filename[0] == 0xE5)
      continue;

    int match = 1;
    for (int j = 0; j < 8; j++) {
      if (entry->filename[j] != target_name[j]) {
        match = 0;
        break;
      }
    }

    if (match) {
      for (int j = 0; j < 3; j++) {
        if (entry->ext[j] != target_ext[j]) {
          match = 0;
          break;
        }
      }
    }

    if (match) {
      memcpy(out_entry, entry, sizeof(Fat16Entry));
      fs->current_file_index = old_index;
      return 1;
    }
  }

  fs->current_file_index = old_index;
  return 0;
}

static uint16_t get_parent_cluster(FatFileSystem *fs, uint16_t from_cluster) {
  Fat16Entry parent;
  if (from_cluster == 0)
    return 0;

  if (find_entry_in_dir(fs, from_cluster, "..", &parent) &&
      (parent.attributes & 0x10)) {
    return parent.starting_cluster;
  }

  return 0;
}

// expected_type: 0 any, 1 directory, 2 file
static int resolve_path(const char *path, int expected_type,
                        uint16_t *out_cluster, Fat16Entry *out_entry,
                        int *out_is_synthetic_dir) {
  FatFileSystem *fs = &g_fat_fs;
  char parts[PATH_PART_MAX][PATH_PART_LEN];
  int count;
  int absolute;
  uint16_t current_cluster;
  int has_real_entry = 0;
  Fat16Entry last_entry;

  memset(&last_entry, 0, sizeof(Fat16Entry));

  if (!path || path[0] == '\0')
    return 0;

  if (!split_path_tokens(path, parts, &count, &absolute))
    return 0;

  current_cluster = absolute ? 0 : fs->pwd_cluster;

  if (count == 0) {
    if (expected_type == 2)
      return 0;

    if (out_cluster)
      *out_cluster = current_cluster;
    if (out_entry)
      memset(out_entry, 0, sizeof(Fat16Entry));
    if (out_is_synthetic_dir)
      *out_is_synthetic_dir = 1;
    return 1;
  }

  for (int i = 0; i < count; i++) {
    int last = (i == count - 1);
    char *part = parts[i];

    if (strcmp(part, ".") == 0)
      continue;

    if (strcmp(part, "..") == 0) {
      current_cluster = get_parent_cluster(fs, current_cluster);
      if (last) {
        has_real_entry = 0;
      }
      continue;
    }

    Fat16Entry found;
    if (!find_entry_in_dir(fs, current_cluster, part, &found))
      return 0;

    if (!last) {
      if (!(found.attributes & 0x10))
        return 0;
      current_cluster = found.starting_cluster;
      continue;
    }

    memcpy(&last_entry, &found, sizeof(Fat16Entry));
    has_real_entry = 1;
  }

  if (has_real_entry) {
    int is_dir = (last_entry.attributes & 0x10) != 0;
    if (expected_type == 1 && !is_dir)
      return 0;
    if (expected_type == 2 && is_dir)
      return 0;

    if (out_cluster) {
      *out_cluster = is_dir ? last_entry.starting_cluster : current_cluster;
    }
    if (out_entry)
      memcpy(out_entry, &last_entry, sizeof(Fat16Entry));
    if (out_is_synthetic_dir)
      *out_is_synthetic_dir = 0;
    return 1;
  }

  if (expected_type == 2)
    return 0;

  if (out_cluster)
    *out_cluster = current_cluster;
  if (out_entry)
    memset(out_entry, 0, sizeof(Fat16Entry));
  if (out_is_synthetic_dir)
    *out_is_synthetic_dir = 1;
  return 1;
}

static void normalize_path(const char *base, const char *input, char *out,
                           int out_len) {
  char base_parts[PATH_PART_MAX][PATH_PART_LEN];
  char input_parts[PATH_PART_MAX][PATH_PART_LEN];
  char stack[PATH_PART_MAX][PATH_PART_LEN];
  int base_count = 0;
  int input_count = 0;
  int base_abs = 1;
  int input_abs = 0;
  int stack_count = 0;

  if (!out || out_len < 2)
    return;

  out[0] = '/';
  out[1] = '\0';

  if (!input || input[0] == '\0')
    return;

  split_path_tokens(base, base_parts, &base_count, &base_abs);
  split_path_tokens(input, input_parts, &input_count, &input_abs);

  if (!input_abs) {
    for (int i = 0; i < base_count && i < PATH_PART_MAX; i++) {
      strcpy(stack[stack_count++], base_parts[i]);
    }
  }

  for (int i = 0; i < input_count; i++) {
    if (strcmp(input_parts[i], ".") == 0)
      continue;
    if (strcmp(input_parts[i], "..") == 0) {
      if (stack_count > 0)
        stack_count--;
      continue;
    }

    if (stack_count < PATH_PART_MAX) {
      strcpy(stack[stack_count++], input_parts[i]);
    }
  }

  int pos = 0;
  out[pos++] = '/';

  for (int i = 0; i < stack_count; i++) {
    int len = (int)strlen(stack[i]);
    if (pos + len + 1 >= out_len)
      break;

    memcpy(out + pos, stack[i], (size_t)len);
    pos += len;

    if (i + 1 < stack_count)
      out[pos++] = '/';
  }

  out[pos] = '\0';
}

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
  vga_print("  ls                   - List files in current directory\n");
  vga_print("  cat <file>           - Print file contents\n");
  vga_print("  cd <path>            - Change current directory\n");
  vga_print("  tree                 - Recursively list current directory\n");
  vga_print("  stat <path>          - Show file or directory metadata\n");
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

static void do_ls() {
  FatFileSystem *fs = &g_fat_fs;
  int old_index = fs->current_file_index;
  uint32_t max_entries;

  if (fs->selected_partition_table == NULL) {
    vga_print("FAT16 filesystem is not mounted.\n");
    fs->current_file_index = old_index;
    return;
  }

  vga_print("Current directory:\n");
  fs->current_file_index = -1;

  if (fs->pwd_cluster == 0) {
    max_entries = fs->boot_sector.root_dir_entries;
  } else {
    max_entries = 4096;
  }

  for (uint32_t entry_index = 0; entry_index < max_entries; entry_index++) {
    fat_read_directory_entry(fs, fs->pwd_cluster);
    Fat16Entry *entry = &fs->current_file;

    if (entry->filename[0] == 0x00) {
      break;
    }

    if ((unsigned char)entry->filename[0] == 0xE5) {
      continue;
    }

    if (entry->filename[0] == '.') {
      continue;
    }

    if (entry->attributes == 0x0F || (entry->attributes & 0x08)) {
      continue;
    }

    vga_print(entry->attributes & 0x10 ? "<DIR>  " : "FILE   ");
    print_fat_name(entry);

    if (!(entry->attributes & 0x10)) {
      vga_print("  ");
      print_dec(entry->file_size);
      vga_print(" B");
    }

    vga_print("\n");
  }

  fs->current_file_index = old_index;
}

static void do_cat(const char *path) {
  Fat16Entry entry;
  void *buffer = NULL;
  uint32_t size = 0;
  int synthetic = 0;

  if (!resolve_path(path, 2, NULL, &entry, &synthetic)) {
    vga_print("cat: file not found: ");
    vga_print(path);
    vga_print("\n");
    return;
  }

  if (synthetic || (entry.attributes & 0x10)) {
    vga_print("cat: target is a directory\n");
    return;
  }

  if (!fat_read_file(&g_fat_fs, &entry, &buffer, &size) || !buffer) {
    vga_print("cat: failed to read file\n");
    return;
  }

  uint8_t *bytes = (uint8_t *)buffer;
  for (uint32_t i = 0; i < size; i++) {
    char c = (char)bytes[i];
    if (c == '\n' || c == '\r' || c == '\t' || (c >= 32 && c <= 126)) {
      vga_putchar(c);
    } else {
      vga_putchar('.');
    }
  }

  if (size == 0 || bytes[size - 1] != '\n') {
    vga_putchar('\n');
  }
}

static void do_cd(const char *path) {
  uint16_t next_cluster;
  char normalized[128];

  if (!resolve_path(path, 1, &next_cluster, NULL, NULL)) {
    vga_print("cd: no such directory: ");
    vga_print(path);
    vga_print("\n");
    return;
  }

  g_fat_fs.pwd_cluster = next_cluster;
  normalize_path(cli_current_path, path, normalized, (int)sizeof(normalized));
  strcpy(cli_current_path, normalized);
}

static void do_tree_recursive(uint16_t cluster, int depth) {
  FatFileSystem *fs = &g_fat_fs;
  int old_index = fs->current_file_index;
  uint32_t max_entries =
      (cluster == 0) ? fs->boot_sector.root_dir_entries : 65535;

  fs->current_file_index = -1;

  for (uint32_t i = 0; i < max_entries; i++) {
    fat_read_directory_entry(fs, cluster);
    Fat16Entry *entry = &fs->current_file;

    if (entry->filename[0] == 0x00)
      break;

    if ((unsigned char)entry->filename[0] == 0xE5)
      continue;

    if (entry->filename[0] == '.')
      continue;

    if (entry->attributes == 0x0F || (entry->attributes & 0x08))
      continue;

    for (int d = 0; d < depth; d++)
      vga_print("  ");
    vga_print("|- ");
    print_fat_name(entry);

    if (entry->attributes & 0x10) {
      vga_print("/\n");
      do_tree_recursive(entry->starting_cluster, depth + 1);
    } else {
      vga_print(" (");
      print_dec(entry->file_size);
      vga_print(" B)\n");
    }
  }

  fs->current_file_index = old_index;
}

static void do_tree() {
  vga_print(".\n");
  do_tree_recursive(g_fat_fs.pwd_cluster, 0);
}

static void do_stat(const char *path) {
  Fat16Entry entry;
  uint16_t cluster = 0;
  int synthetic = 0;

  if (!resolve_path(path, 0, &cluster, &entry, &synthetic)) {
    vga_print("stat: target not found: ");
    vga_print(path);
    vga_print("\n");
    return;
  }

  vga_print("Path: ");
  vga_print(path);
  vga_print("\nType: ");

  if (synthetic || (entry.attributes & 0x10)) {
    vga_print("directory\n");
    vga_print("Name: ");
    if (strcmp(path, "/") == 0 || strcmp(path, ".") == 0 ||
        strcmp(path, "..") == 0)
      vga_print("/\n");
    else {
      print_fat_name(&entry);
      vga_print("\n");
    }
    vga_print("Cluster: ");
    print_dec(cluster);
    vga_print("\n");
    if (!synthetic) {
      vga_print("Attributes: 0x");
      print_hex(entry.attributes, 2);
      vga_print("\nModified: ");
      print_fat_datetime(entry.modify_date, entry.modify_time);
      vga_print("\n");
    }
    return;
  }

  vga_print("file\n");
  vga_print("Name: ");
  print_fat_name(&entry);
  vga_print("\nSize: ");
  print_dec(entry.file_size);
  vga_print(" B\n");
  vga_print("Starting Cluster: ");
  print_dec(entry.starting_cluster);
  vga_print("\nAttributes: 0x");
  print_hex(entry.attributes, 2);
  vga_print("\nModified: ");
  print_fat_datetime(entry.modify_date, entry.modify_time);
  vga_print("\n");
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
    vga_print(cli_current_path);
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
    } else if (strcmp(argv[0], "ls") == 0) {
      do_ls();
    } else if (strcmp(argv[0], "cat") == 0) {
      if (argc >= 2) {
        do_cat(argv[1]);
      } else {
        vga_print("Usage: cat <filename>\n");
      }
    } else if (strcmp(argv[0], "cd") == 0) {
      if (argc >= 2) {
        do_cd(argv[1]);
      } else {
        vga_print("Usage: cd <path>\n");
      }
    } else if (strcmp(argv[0], "tree") == 0) {
      do_tree();
    } else if (strcmp(argv[0], "stat") == 0) {
      if (argc >= 2) {
        do_stat(argv[1]);
      } else {
        vga_print("Usage: stat <path>\n");
      }
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
