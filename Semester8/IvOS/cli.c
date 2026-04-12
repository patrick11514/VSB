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

#define APP_LOAD_ADDR 0x100000
#define APP_MAX_SIZE (512 * 1024)
#define CLI_WRITE_BUFFER_SIZE (64 * 1024)

// --- CLI State ---
static uint8_t disk_buffer[512];
static uint8_t write_buffer[CLI_WRITE_BUFFER_SIZE];
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

static int split_parent_leaf(const char *path, char *parent, int parent_len,
                             char *leaf, int leaf_len) {
  int last_slash = -1;
  int len = 0;

  if (!path || !parent || !leaf || parent_len <= 0 || leaf_len <= 0)
    return 0;

  while (path[len] != '\0') {
    if (path[len] == '/')
      last_slash = len;
    len++;
  }

  if (len == 0)
    return 0;

  if (last_slash < 0) {
    if (len >= leaf_len)
      return 0;
    parent[0] = '\0';
    memcpy(leaf, path, len + 1);
    return 1;
  }

  if (last_slash == len - 1)
    return 0;

  if (last_slash == 0) {
    if (parent_len < 2)
      return 0;
    parent[0] = '/';
    parent[1] = '\0';
  } else {
    if (last_slash >= parent_len)
      return 0;
    memcpy(parent, path, (size_t)last_slash);
    parent[last_slash] = '\0';
  }

  int leaf_len_value = len - last_slash - 1;
  if (leaf_len_value >= leaf_len)
    return 0;
  memcpy(leaf, path + last_slash + 1, (size_t)leaf_len_value);
  leaf[leaf_len_value] = '\0';
  return 1;
}

static int resolve_parent_cluster(const char *path, uint16_t *out_cluster,
                                  char *leaf, int leaf_len) {
  char parent[128];

  if (!split_parent_leaf(path, parent, (int)sizeof(parent), leaf, leaf_len))
    return 0;

  if (parent[0] == '\0') {
    if (out_cluster)
      *out_cluster = g_fat_fs.pwd_cluster;
    return 1;
  }

  return resolve_path(parent, 1, out_cluster, NULL, NULL);
}

static int resolve_existing_file(const char *path, uint16_t *out_parent_cluster,
                                 char *leaf, int leaf_len,
                                 Fat16Entry *out_entry) {
  uint16_t parent_cluster = 0;

  if (!resolve_parent_cluster(path, &parent_cluster, leaf, leaf_len))
    return 0;

  if (!find_entry_in_dir(&g_fat_fs, parent_cluster, leaf, out_entry))
    return 0;

  if (out_entry->attributes & 0x10)
    return 0;

  if (out_parent_cluster)
    *out_parent_cluster = parent_cluster;
  return 1;
}

static int write_bytes_to_dir(uint16_t parent_cluster, const char *leaf,
                              const void *buffer, uint32_t size) {
  uint16_t saved_pwd = g_fat_fs.pwd_cluster;
  Fat16Entry existing;
  int result;

  g_fat_fs.pwd_cluster = parent_cluster;

  if (find_entry_in_dir(&g_fat_fs, parent_cluster, leaf, &existing)) {
    if (existing.attributes & 0x10) {
      g_fat_fs.pwd_cluster = saved_pwd;
      return 0;
    }
    if (!fat_delete(&g_fat_fs, leaf)) {
      g_fat_fs.pwd_cluster = saved_pwd;
      return 0;
    }
  }

  result = fat_write(&g_fat_fs, leaf, buffer, size);
  g_fat_fs.pwd_cluster = saved_pwd;
  return result;
}

static uint32_t collect_write_input(uint8_t *buffer, uint32_t max_size) {
  uint32_t size = 0;

  while (1) {
    char c = keyboard_getchar();

    if (c == 4) {
      vga_putchar('\n');
      return size;
    }

    if (c == '\r') {
      c = '\n';
    }

    if (c == '\b' || c == 8) {
      if (size > 0) {
        size--;
        vga_putchar('\b');
      }
      continue;
    }

    if (c == '\n' || c == '\t' || (c >= 32 && c <= 126)) {
      if (size >= max_size) {
        vga_print("\nwrite: input too large\n");
        return 0xFFFFFFFF;
      }

      buffer[size++] = (uint8_t)c;
      vga_putchar(c);
    }
  }
}

static void do_write_file(const char *path) {
  uint16_t parent_cluster = 0;
  char leaf[128];
  Fat16Entry existing;
  uint32_t size;

  if (!resolve_parent_cluster(path, &parent_cluster, leaf, (int)sizeof(leaf))) {
    vga_print("write: invalid path\n");
    return;
  }

  if (!make_fat_83_name(leaf, existing.filename, existing.ext)) {
    vga_print("write: invalid file name\n");
    return;
  }

  if (find_entry_in_dir(&g_fat_fs, parent_cluster, leaf, &existing) &&
      (existing.attributes & 0x10)) {
    vga_print("write: target is a directory\n");
    return;
  }

  vga_print("Enter content, Ctrl+D to finish.\n");
  size = collect_write_input(write_buffer, CLI_WRITE_BUFFER_SIZE);
  if (size == 0xFFFFFFFF)
    return;

  if (!write_bytes_to_dir(parent_cluster, leaf, write_buffer, size)) {
    vga_print("write: failed to write file\n");
  }
}

static void do_touch(const char *path) {
  uint16_t parent_cluster = 0;
  char leaf[128];
  Fat16Entry existing;
  static uint8_t empty_buffer;

  if (!resolve_parent_cluster(path, &parent_cluster, leaf, (int)sizeof(leaf))) {
    vga_print("touch: invalid path\n");
    return;
  }

  if (find_entry_in_dir(&g_fat_fs, parent_cluster, leaf, &existing) &&
      (existing.attributes & 0x10)) {
    vga_print("touch: target is a directory\n");
    return;
  }

  if (!write_bytes_to_dir(parent_cluster, leaf, &empty_buffer, 0)) {
    vga_print("touch: failed to create file\n");
  }
}

static void do_rm(const char *path) {
  uint16_t parent_cluster = 0;
  char leaf[128];
  Fat16Entry existing;
  uint16_t saved_pwd;

  if (!resolve_existing_file(path, &parent_cluster, leaf, (int)sizeof(leaf),
                             &existing)) {
    vga_print("rm: file not found: ");
    vga_print(path);
    vga_print("\n");
    return;
  }

  saved_pwd = g_fat_fs.pwd_cluster;
  g_fat_fs.pwd_cluster = parent_cluster;
  if (!fat_delete(&g_fat_fs, leaf)) {
    vga_print("rm: failed to delete file\n");
  }
  g_fat_fs.pwd_cluster = saved_pwd;
}

static void do_mkdir(const char *path) {
  uint16_t parent_cluster = 0;
  char leaf[128];
  Fat16Entry existing;
  uint16_t saved_pwd;

  if (!resolve_parent_cluster(path, &parent_cluster, leaf, (int)sizeof(leaf))) {
    vga_print("mkdir: invalid path\n");
    return;
  }

  if (find_entry_in_dir(&g_fat_fs, parent_cluster, leaf, &existing)) {
    vga_print("mkdir: target already exists\n");
    return;
  }

  saved_pwd = g_fat_fs.pwd_cluster;
  g_fat_fs.pwd_cluster = parent_cluster;
  if (!fat_mkdir(&g_fat_fs, leaf)) {
    vga_print("mkdir: failed to create directory\n");
  }
  g_fat_fs.pwd_cluster = saved_pwd;
}

static void do_rmdir(const char *path) {
  uint16_t parent_cluster = 0;
  char leaf[128];
  Fat16Entry existing;
  uint16_t saved_pwd;

  if (!resolve_parent_cluster(path, &parent_cluster, leaf, (int)sizeof(leaf))) {
    vga_print("rmdir: invalid path\n");
    return;
  }

  if (!find_entry_in_dir(&g_fat_fs, parent_cluster, leaf, &existing)) {
    vga_print("rmdir: directory not found: ");
    vga_print(path);
    vga_print("\n");
    return;
  }

  if (!(existing.attributes & 0x10)) {
    vga_print("rmdir: target is not a directory\n");
    return;
  }

  saved_pwd = g_fat_fs.pwd_cluster;
  g_fat_fs.pwd_cluster = parent_cluster;
  if (!fat_rmdir(&g_fat_fs, leaf)) {
    vga_print("rmdir: directory is not empty or cannot be removed\n");
  }
  g_fat_fs.pwd_cluster = saved_pwd;
}

static void do_cp(const char *src_path, const char *dst_path) {
  uint16_t src_parent = 0;
  uint16_t dst_parent = 0;
  char src_leaf[128];
  char dst_leaf[128];
  Fat16Entry src_entry;
  Fat16Entry dst_entry;
  void *buffer = NULL;
  uint32_t size = 0;
  int synthetic = 0;
  uint16_t saved_pwd;

  if (!resolve_existing_file(src_path, &src_parent, src_leaf,
                             (int)sizeof(src_leaf), &src_entry)) {
    vga_print("cp: source file not found: ");
    vga_print(src_path);
    vga_print("\n");
    return;
  }

  if (resolve_path(dst_path, 1, &dst_parent, &dst_entry, &synthetic)) {
    strcpy(dst_leaf, src_leaf);
  } else {
    if (!resolve_parent_cluster(dst_path, &dst_parent, dst_leaf,
                                (int)sizeof(dst_leaf))) {
      vga_print("cp: invalid destination path\n");
      return;
    }
    if (find_entry_in_dir(&g_fat_fs, dst_parent, dst_leaf, &dst_entry)) {
      if (dst_entry.attributes & 0x10) {
        vga_print("cp: destination is a directory\n");
        return;
      }
    }
  }

  if (src_parent == dst_parent && strcmp(src_leaf, dst_leaf) == 0) {
    return;
  }

  if (!fat_read_file(&g_fat_fs, &src_entry, &buffer, &size) || !buffer) {
    vga_print("cp: failed to read source file\n");
    return;
  }

  saved_pwd = g_fat_fs.pwd_cluster;
  g_fat_fs.pwd_cluster = dst_parent;
  if (!write_bytes_to_dir(dst_parent, dst_leaf, buffer, size)) {
    vga_print("cp: failed to write destination file\n");
  }
  g_fat_fs.pwd_cluster = saved_pwd;
}

static void do_mv(const char *src_path, const char *dst_path) {
  uint16_t src_parent = 0;
  uint16_t dst_parent = 0;
  char src_leaf[128];
  char dst_leaf[128];
  Fat16Entry src_entry;
  Fat16Entry dst_entry;
  void *buffer = NULL;
  uint32_t size = 0;
  int synthetic = 0;
  uint16_t saved_pwd;

  if (!resolve_existing_file(src_path, &src_parent, src_leaf,
                             (int)sizeof(src_leaf), &src_entry)) {
    vga_print("mv: source file not found: ");
    vga_print(src_path);
    vga_print("\n");
    return;
  }

  if (resolve_path(dst_path, 1, &dst_parent, &dst_entry, &synthetic)) {
    strcpy(dst_leaf, src_leaf);
  } else {
    if (!resolve_parent_cluster(dst_path, &dst_parent, dst_leaf,
                                (int)sizeof(dst_leaf))) {
      vga_print("mv: invalid destination path\n");
      return;
    }
    if (find_entry_in_dir(&g_fat_fs, dst_parent, dst_leaf, &dst_entry)) {
      if (dst_entry.attributes & 0x10) {
        vga_print("mv: destination is a directory\n");
        return;
      }
    }
  }

  if (src_parent == dst_parent && strcmp(src_leaf, dst_leaf) == 0) {
    return;
  }

  if (!fat_read_file(&g_fat_fs, &src_entry, &buffer, &size) || !buffer) {
    vga_print("mv: failed to read source file\n");
    return;
  }

  saved_pwd = g_fat_fs.pwd_cluster;
  g_fat_fs.pwd_cluster = dst_parent;
  if (!write_bytes_to_dir(dst_parent, dst_leaf, buffer, size)) {
    vga_print("mv: failed to write destination file\n");
    g_fat_fs.pwd_cluster = saved_pwd;
    return;
  }
  g_fat_fs.pwd_cluster = saved_pwd;

  g_fat_fs.pwd_cluster = src_parent;
  if (!fat_delete(&g_fat_fs, src_leaf)) {
    vga_print(
        "mv: warning: destination written, but source could not be removed\n");
  }
  g_fat_fs.pwd_cluster = saved_pwd;
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
  vga_print(
      "  write <file>         - Overwrite/create file from typed input\n");
  vga_print("  touch <file>         - Create an empty file\n");
  vga_print("  rm <file>            - Delete a file\n");
  vga_print("  mkdir <dir>          - Create a directory\n");
  vga_print("  rmdir <dir>          - Remove an empty directory\n");
  vga_print("  cp <src> <dst>       - Copy a file\n");
  vga_print("  mv <src> <dst>       - Move a file\n");
  vga_print("  read <lba>           - Read sector into internal buffer\n");
  vga_print("  wsect <lba>          - Write internal buffer to sector\n");
  vga_print("  dump <addr>          - Hex dump 128 bytes from addr\n");
  vga_print("  load <lba> <addr>    - Load 1 sector from lba to addr\n");
  vga_print("  list                 - List available programs in /games\n");
  vga_print(
      "  exec <path>          - Load and run program from current path\n");
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
  FatFileSystem *fs = &g_fat_fs;
  uint16_t games_cluster = 0;
  int old_index = fs->current_file_index;
  uint32_t max_entries;

  if (!resolve_path("/games", 1, &games_cluster, NULL, NULL)) {
    vga_print("/games directory not found.\n");
    return;
  }

  vga_print("Programs in /games:\n");
  fs->current_file_index = -1;
  max_entries = (games_cluster == 0) ? fs->boot_sector.root_dir_entries : 65535;

  for (uint32_t i = 0; i < max_entries; i++) {
    fat_read_directory_entry(fs, games_cluster);
    Fat16Entry *entry = &fs->current_file;

    if (entry->filename[0] == 0x00)
      break;

    if ((unsigned char)entry->filename[0] == 0xE5)
      continue;

    if (entry->filename[0] == '.')
      continue;

    if (entry->attributes == 0x0F || (entry->attributes & 0x08))
      continue;

    if (entry->attributes & 0x10)
      continue;

    vga_print("  ");
    print_fat_name(entry);
    vga_print("  ");
    print_dec(entry->file_size);
    vga_print(" B\n");
  }

  fs->current_file_index = old_index;
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
  static char full_path[160];
  static Fat16Entry entry;
  void *buffer = NULL;
  uint32_t size = 0;

  if (target_name == NULL || strlen(target_name) == 0) {
    vga_print("Usage: exec <filename>\n");
    return;
  }

  if (target_name[0] == '/') {
    if (strlen(target_name) + 1 >= sizeof(full_path)) {
      vga_print("exec: path too long\n");
      return;
    }
    strcpy(full_path, target_name);
  } else {
    if (strcmp(cli_current_path, "/") == 0) {
      if (strlen(target_name) + 2 >= sizeof(full_path)) {
        vga_print("exec: path too long\n");
        return;
      }
      strcpy(full_path, "/");
      strcat(full_path, target_name);
    } else {
      if (strlen(cli_current_path) + strlen(target_name) + 2 >=
          sizeof(full_path)) {
        vga_print("exec: path too long\n");
        return;
      }
      strcpy(full_path, cli_current_path);
      strcat(full_path, "/");
      strcat(full_path, target_name);
    }
  }

  if (!resolve_path(full_path, 2, NULL, &entry, NULL)) {
    vga_print("Program not found: ");
    vga_print(target_name);
    vga_print("\n");
    return;
  }

  if (!fat_read_file(&g_fat_fs, &entry, &buffer, &size) || !buffer) {
    vga_print("exec: failed to read app file\n");
    return;
  }

  if (size == 0 || size > APP_MAX_SIZE) {
    vga_print("exec: invalid app size\n");
    return;
  }

  memcpy((void *)APP_LOAD_ADDR, buffer, size);

  void (*func)() = (void (*)())APP_LOAD_ADDR;
  func();

  vga_print("Program finished.\n");
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
    } else if (strcmp(argv[0], "write") == 0) {
      if (argc >= 2) {
        do_write_file(argv[1]);
      } else {
        vga_print("Usage: write <file>\n");
      }
    } else if (strcmp(argv[0], "touch") == 0) {
      if (argc >= 2) {
        do_touch(argv[1]);
      } else {
        vga_print("Usage: touch <file>\n");
      }
    } else if (strcmp(argv[0], "rm") == 0) {
      if (argc >= 2) {
        do_rm(argv[1]);
      } else {
        vga_print("Usage: rm <file>\n");
      }
    } else if (strcmp(argv[0], "mkdir") == 0) {
      if (argc >= 2) {
        do_mkdir(argv[1]);
      } else {
        vga_print("Usage: mkdir <dir>\n");
      }
    } else if (strcmp(argv[0], "rmdir") == 0) {
      if (argc >= 2) {
        do_rmdir(argv[1]);
      } else {
        vga_print("Usage: rmdir <dir>\n");
      }
    } else if (strcmp(argv[0], "cp") == 0) {
      if (argc >= 3) {
        do_cp(argv[1], argv[2]);
      } else {
        vga_print("Usage: cp <src> <dst>\n");
      }
    } else if (strcmp(argv[0], "mv") == 0) {
      if (argc >= 3) {
        do_mv(argv[1], argv[2]);
      } else {
        vga_print("Usage: mv <src> <dst>\n");
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
    } else if (strcmp(argv[0], "wsect") == 0) {
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
        vga_print("Usage: wsect <lba>\n");
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
    } else if (strcmp(argv[0], "list") == 0) {
      do_list();
    } else if (strcmp(argv[0], "exec") == 0) {
      if (argc >= 2) {
        do_exec(argv[1]);
      } else {
        vga_print("Usage: exec <path>\n");
      }
    } else {
      vga_print("Unknown command: ");
      vga_print(argv[0]);
      vga_print("\n");
    }
  }
}
