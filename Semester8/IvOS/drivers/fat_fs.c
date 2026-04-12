#include "fat_fs.h"

#include "../drivers/serial.h"
#include "../lib/string.h"

static char fat_to_upper(char c) {
  if (c >= 'a' && c <= 'z') {
    return (char)(c - ('a' - 'A'));
  }
  return c;
}

void fat_read_partitions(FatFileSystem *self, PartitionTable *pt) {
  (void)self;
  read_at_byte(0x1BE, pt, sizeof(PartitionTable) * 4);
}

void fat_select_partition_table(FatFileSystem *self, PartitionTable *pt) {
  self->selected_partition_table = pt;
}

void fat_read_boot_sector(FatFileSystem *self) {
  if (!self->selected_partition_table) {
    serial_print("No partition table selected\n");
    return;
  }

  read_at_byte(self->selected_partition_table->start_sector * 512,
               &self->boot_sector, sizeof(Fat16BootSector));
}

static uint16_t get_fat_entry(FatFileSystem *self, uint16_t cluster) {
  uint32_t fat_start_sector = get_fat_start_sector(
      self->selected_partition_table, &self->boot_sector, 0);
  uint32_t offset_bytes = cluster * 2;
  uint32_t sector_offset = offset_bytes / 512;
  uint32_t byte_in_sector = offset_bytes % 512;

  uint16_t next_cluster = 0;
  read_at_byte((fat_start_sector + sector_offset) * 512 + byte_in_sector,
               &next_cluster, 2);

  return next_cluster;
}

void fat_read_directory_entry(FatFileSystem *self, uint32_t dir_cluster) {
  serial_print("[fat/read_dir_entry] enter\n");

  if (!self->selected_partition_table) {
    serial_print("No partition table selected\n");
    return;
  }

  self->current_file_index++;

  serial_print("[fat/read_dir_entry] incremented index\n");

  int entries_per_sector = 512 / sizeof(Fat16Entry);
  int entries_per_cluster =
      entries_per_sector * self->boot_sector.sectors_per_cluster;

  if (dir_cluster == 0) {
    serial_print("[fat/read_dir_entry] root dir branch\n");
    int root_dir_start_sector = get_root_dir_start_sector(
        self->selected_partition_table, &self->boot_sector);
    self->current_entry_offset =
        root_dir_start_sector * 512 +
        (self->current_file_index * sizeof(Fat16Entry));

    serial_print("[fat/read_dir_entry] calling read_at_byte\n");
    read_at_byte(self->current_entry_offset, &self->current_file,
                 sizeof(Fat16Entry));
    serial_print("[fat/read_dir_entry] read_at_byte done\n");
  } else {
    serial_print("[fat/read_dir_entry] subdir branch\n");
    uint16_t current_cluster = dir_cluster;
    int local_index = self->current_file_index;

    while (local_index >= entries_per_cluster) {
      current_cluster = get_fat_entry(self, current_cluster);

      if (current_cluster >= 0xFFF8) {
        memset(&self->current_file, 0, sizeof(Fat16Entry));
        return;
      }
      local_index -= entries_per_cluster;
    }

    int data_sectors_start =
        get_root_dir_start_sector(self->selected_partition_table,
                                  &self->boot_sector) +
        (self->boot_sector.root_dir_entries * sizeof(Fat16Entry)) / 512;

    int cluster_sector =
        data_sectors_start +
        (current_cluster - 2) * self->boot_sector.sectors_per_cluster;
    self->current_entry_offset =
        cluster_sector * 512 + (local_index * sizeof(Fat16Entry));

    read_at_byte(self->current_entry_offset, &self->current_file,
                 sizeof(Fat16Entry));
  }
}

int fat_find_file(FatFileSystem *self, const char *name, const char *ext,
                  Fat16Entry *out_entry) {
  serial_print("[fat/find_file] enter\n");

  char target_name[8] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
  char target_ext[3] = {' ', ' ', ' '};

  for (int i = 0; i < 8 && name[i] != '\0'; i++)
    target_name[i] = fat_to_upper(name[i]);
  for (int i = 0; i < 3 && ext[i] != '\0'; i++)
    target_ext[i] = fat_to_upper(ext[i]);

  serial_print("[fat/find_file] uppercased names\n");

  self->current_file_index = -1;

  int max_entries =
      (self->pwd_cluster == 0) ? self->boot_sector.root_dir_entries : 65535;

  serial_print("[fat/find_file] starting loop\n");

  for (int i = 0; i < max_entries; i++) {
    serial_print("[fat/find_file] loop iter\n");
    fat_read_directory_entry(self, self->pwd_cluster);
    serial_print("[fat/find_file] read entry\n");
    Fat16Entry entry = self->current_file;

    if (entry.filename[0] == 0x00) {
      serial_print("[fat/find_file] hit end marker\n");
      break;
    }

    if ((unsigned char)entry.filename[0] == 0xE5)
      continue;

    int match = 1;
    for (int j = 0; j < 8; j++)
      if (fat_to_upper(entry.filename[j]) != target_name[j])
        match = 0;
    for (int j = 0; j < 3; j++)
      if (fat_to_upper(entry.ext[j]) != target_ext[j])
        match = 0;

    if (match) {
      *out_entry = entry;
      serial_print("[fat/find_file] found match\n");
      return 1;
    }
  }
  serial_print("[fat/find_file] not found\n");
  return 0;
}

int fat_read_file(FatFileSystem *self, Fat16Entry *entry, void **out_buffer,
                  uint32_t *out_size) {
  if (!entry || !out_buffer || !out_size || entry->starting_cluster < 2) {
    return 0;
  }

  fat_memory_reset();

  uint8_t *output = (uint8_t *)allocate_memory(entry->file_size);
  if (!output) {
    return 0;
  }

  uint16_t current_cluster = entry->starting_cluster;
  uint32_t remaining_bytes = entry->file_size;

  uint32_t data_start_sector =
      get_root_dir_start_sector(self->selected_partition_table,
                                &self->boot_sector) +
      (self->boot_sector.root_dir_entries * sizeof(Fat16Entry)) / 512;

  uint8_t buffer[512];
  uint8_t *output_cursor = output;

  while (current_cluster >= 2 && current_cluster <= 0xFFEF &&
         remaining_bytes > 0) {
    uint32_t cluster_sector =
        data_start_sector +
        (current_cluster - 2) * self->boot_sector.sectors_per_cluster;

    for (int i = 0;
         i < self->boot_sector.sectors_per_cluster && remaining_bytes > 0;
         i++) {
      read_at_byte((cluster_sector + i) * 512, buffer, 512);

      uint32_t to_copy = (remaining_bytes > 512) ? 512 : remaining_bytes;
      memcpy(output_cursor, buffer, to_copy);
      output_cursor += to_copy;
      remaining_bytes -= to_copy;
    }

    current_cluster = get_fat_entry(self, current_cluster);
  }

  *out_buffer = output;
  *out_size = entry->file_size;
  return 1;
}

int fat_change_dir(FatFileSystem *self, const char *dir_name) {
  Fat16Entry entry;
  if (!fat_find_file(self, dir_name, "", &entry)) {
    return 0;
  }

  if (!(entry.attributes & 0x10)) {
    return 0;
  }

  self->pwd_cluster = entry.starting_cluster;
  return 1;
}

static void _print_dir(FatFileSystem *self, uint16_t start_cluster,
                       uint8_t depth, int classic) {
  int old_index = self->current_file_index;
  self->current_file_index = -1;

  if (depth == 0) {
    serial_print("/\n");
  }

  while (1) {
    fat_read_directory_entry(self, start_cluster);
    Fat16Entry sub_entry = self->current_file;

    if (sub_entry.filename[0] == 0x00)
      break;
    if (sub_entry.filename[0] == 0xE5)
      continue;

    if (sub_entry.filename[0] == '.') {
      continue;
    }

    for (uint8_t i = 0; i < depth; ++i) {
      serial_print("|   ");
    }

    serial_print("|-- ");

    if (classic) {
      prety_print_name((char *)sub_entry.filename, (char *)sub_entry.ext);
    } else {
      fat16entry_to_str(&sub_entry);
      serial_putchar('\n');
    }

    if (sub_entry.attributes & 0x10) {
      if (classic) {
        serial_print("/\n");
      }
      _print_dir(self, sub_entry.starting_cluster, depth + 1, classic);
      continue;
    }

    if (classic) {
      serial_putchar(' ');
      print_file_stat(&sub_entry, 1, 1);
      serial_putchar('\n');
    }
  }

  self->current_file_index = old_index;
}

void fat_print_root_dir(FatFileSystem *self) { _print_dir(self, 0, 0, 1); }

void fat_print_dir(FatFileSystem *self, uint16_t start_cluster) {
  _print_dir(self, start_cluster, 0, 1);
}

void fat_init(FatFileSystem *fs) {
  fs->selected_partition_table = NULL;
  fs->current_file_index = -1;
  fs->pwd_cluster = 0;
  fat_memory_reset();
}

static void set_fat_entry(FatFileSystem *self, uint16_t cluster,
                          uint16_t value) {
  for (int i = 0; i < self->boot_sector.number_of_fats; i++) {
    uint32_t fat_start_sector = get_fat_start_sector(
        self->selected_partition_table, &self->boot_sector, i);
    uint32_t offset_bytes = cluster * 2;
    write_at_byte(fat_start_sector * 512 + offset_bytes, &value, 2);
  }
}

static uint16_t find_free_cluster(FatFileSystem *self) {
  serial_print("[fat/find_free_cluster] enter\n");
  /* Safety limit: scan up to 1000 clusters (should be plenty for our use case)
   */
  for (uint16_t i = 2; i <= 1000; i++) {
    if (get_fat_entry(self, i) == 0x0000) {
      serial_print("[fat/find_free_cluster] found\n");
      return i;
    }
  }
  serial_print("[fat/find_free_cluster] none\n");
  return 0;
}

static uint32_t find_free_dir_entry(FatFileSystem *self, uint32_t dir_cluster) {
  int old_index = self->current_file_index;
  self->current_file_index = -1;

  serial_print("[fat/find_free_dir_entry] enter\n");

  int max_entries =
      (dir_cluster == 0) ? self->boot_sector.root_dir_entries : 65535;

  /* Safety limit: don't scan more than actually possible */
  if (max_entries > 512)
    max_entries = 512;

  for (int i = 0; i < max_entries; i++) {
    fat_read_directory_entry(self, dir_cluster);

    if (self->current_file.filename[0] == 0x00 ||
        (unsigned char)self->current_file.filename[0] == 0xE5) {
      uint32_t offset = self->current_entry_offset;
      self->current_file_index = old_index;
      serial_print("[fat/find_free_dir_entry] found\n");
      return offset;
    }
  }
  self->current_file_index = old_index;
  serial_print("[fat/find_free_dir_entry] none\n");
  return 0;
}

static int directory_is_empty(FatFileSystem *self, uint16_t dir_cluster) {
  int old_index = self->current_file_index;
  self->current_file_index = -1;

  serial_print("[fat/directory_is_empty] enter\n");

  int max_entries =
      (dir_cluster == 0) ? self->boot_sector.root_dir_entries : 65535;

  for (int i = 0; i < max_entries; i++) {
    fat_read_directory_entry(self, dir_cluster);

    if (self->current_file.filename[0] == 0x00) {
      break;
    }

    if ((unsigned char)self->current_file.filename[0] == 0xE5) {
      continue;
    }

    if (self->current_file.attributes & 0x08) {
      continue;
    }

    if (self->current_file.filename[0] == '.') {
      continue;
    }

    self->current_file_index = old_index;
    serial_print("[fat/directory_is_empty] not empty\n");
    return 0;
  }

  self->current_file_index = old_index;
  serial_print("[fat/directory_is_empty] empty\n");
  return 1;
}

int fat_mkdir(FatFileSystem *self, const char *dirname) {
  char name[9] = {0};
  char ext[4] = {0};
  const char *dot = strchr(dirname, '.');
  if (dot) {
    int name_len = dot - dirname;
    if (name_len > 8)
      name_len = 8;
    strncpy(name, dirname, name_len);
    strncpy(ext, dot + 1, 3);
  } else {
    strncpy(name, dirname, 8);
  }

  serial_print("[fat/mkdir] enter\n");

  uint32_t entry_offset = find_free_dir_entry(self, self->pwd_cluster);
  if (entry_offset == 0) {
    serial_print("No free directory entry found\n");
    serial_print("[fat/mkdir] no dir entry\n");
    return 0;
  }

  uint16_t new_cluster = find_free_cluster(self);
  if (new_cluster == 0) {
    serial_print("Disk full\n");
    serial_print("[fat/mkdir] no cluster\n");
    return 0;
  }

  set_fat_entry(self, new_cluster, 0xFFFF);

  uint32_t bytes_per_cluster = self->boot_sector.sectors_per_cluster * 512;
  fat_memory_reset();
  uint8_t *cluster_buffer = (uint8_t *)allocate_memory(bytes_per_cluster);
  if (!cluster_buffer) {
    set_fat_entry(self, new_cluster, 0x0000);
    serial_print("[fat/mkdir] alloc failed\n");
    return 0;
  }

  memset(cluster_buffer, 0, bytes_per_cluster);

  uint32_t data_start_sector =
      get_root_dir_start_sector(self->selected_partition_table,
                                &self->boot_sector) +
      (self->boot_sector.root_dir_entries * sizeof(Fat16Entry)) / 512;

  Fat16Entry dot_entry;
  memset(&dot_entry, 0, sizeof(Fat16Entry));
  memset(dot_entry.filename, ' ', 8);
  memset(dot_entry.ext, ' ', 3);
  dot_entry.filename[0] = '.';
  dot_entry.attributes = 0x10;
  dot_entry.starting_cluster = new_cluster;

  Fat16Entry dotdot_entry;
  memset(&dotdot_entry, 0, sizeof(Fat16Entry));
  memset(dotdot_entry.filename, ' ', 8);
  memset(dotdot_entry.ext, ' ', 3);
  dotdot_entry.filename[0] = '.';
  dotdot_entry.filename[1] = '.';
  dotdot_entry.attributes = 0x10;
  dotdot_entry.starting_cluster = self->pwd_cluster;

  memcpy(cluster_buffer, &dot_entry, sizeof(Fat16Entry));
  memcpy(cluster_buffer + sizeof(Fat16Entry), &dotdot_entry,
         sizeof(Fat16Entry));

  uint32_t cluster_sector =
      data_start_sector +
      (new_cluster - 2) * self->boot_sector.sectors_per_cluster;
  write_at_byte(cluster_sector * 512, cluster_buffer, bytes_per_cluster);
  free_memory(cluster_buffer);

  Fat16Entry new_entry;
  memset(&new_entry, 0, sizeof(Fat16Entry));
  memset(new_entry.filename, ' ', 8);
  memset(new_entry.ext, ' ', 3);
  for (int i = 0; i < 8 && name[i]; i++)
    new_entry.filename[i] = fat_to_upper(name[i]);
  for (int i = 0; i < 3 && ext[i]; i++)
    new_entry.ext[i] = fat_to_upper(ext[i]);
  new_entry.attributes = 0x10;
  new_entry.starting_cluster = new_cluster;

  write_at_byte(entry_offset, &new_entry, sizeof(Fat16Entry));
  serial_print("[fat/mkdir] exit\n");
  return 1;
}

int fat_rmdir(FatFileSystem *self, const char *dirname) {
  Fat16Entry entry;
  char name[9] = {0};
  char ext[4] = {0};
  const char *dot = strchr(dirname, '.');
  if (dot) {
    int name_len = dot - dirname;
    if (name_len > 8)
      name_len = 8;
    strncpy(name, dirname, name_len);
    strncpy(ext, dot + 1, 3);
  } else {
    strncpy(name, dirname, 8);
  }

  serial_print("[fat/rmdir] enter\n");

  if (!fat_find_file(self, name, ext, &entry)) {
    serial_print("[fat/rmdir] not found\n");
    return 0;
  }

  if (!(entry.attributes & 0x10)) {
    serial_print("[fat/rmdir] not dir\n");
    return 0;
  }

  if (!directory_is_empty(self, entry.starting_cluster)) {
    serial_print("[fat/rmdir] not empty\n");
    return 0;
  }

  serial_print("[fat/rmdir] deleting\n");
  return fat_delete(self, dirname);
}

int fat_delete(FatFileSystem *self, const char *filename) {
  serial_print("[fat/delete] enter\n");

  Fat16Entry entry;
  char name[9] = {0};
  char ext[4] = {0};
  const char *dot = strchr(filename, '.');

  serial_print("[fat/delete] after strchr\n");

  if (dot) {
    int name_len = dot - filename;
    if (name_len > 8)
      name_len = 8;
    strncpy(name, filename, name_len);
    strncpy(ext, dot + 1, 3);
  } else {
    strncpy(name, filename, 8);
  }

  serial_print("[fat/delete] parsed name/ext\n");

  if (!fat_find_file(self, name, ext, &entry)) {
    serial_print("[fat/delete] not found\n");
    return 0;
  }

  serial_print("[fat/delete] found entry\n");

  fat_delete_entry(self, &entry, self->current_entry_offset);

  serial_print("[fat/delete] exit\n");

  return 1;
}

int fat_delete_entry(FatFileSystem *self, const Fat16Entry *entry,
                     uint32_t entry_offset) {
  Fat16Entry deleted_entry;
  uint16_t cluster;

  serial_print("[fat/delete_entry] enter\n");

  if (!entry) {
    return 0;
  }

  serial_print("[fat/delete_entry] got entry\n");

  cluster = entry->starting_cluster;
  serial_print("[fat/delete_entry] cluster loaded\n");

  while (cluster >= 2 && cluster <= 0xFFEF) {
    serial_print("[fat/delete_entry] marking cluster free\n");
    uint16_t next_cluster = get_fat_entry(self, cluster);
    set_fat_entry(self, cluster, 0x0000);
    cluster = next_cluster;
  }

  read_at_byte(entry_offset, &deleted_entry, sizeof(Fat16Entry));
  deleted_entry.filename[0] = 0xE5;
  write_at_byte(entry_offset, &deleted_entry, sizeof(Fat16Entry));
  serial_print("[fat/delete_entry] entry deleted\n");
  return 1;
}

int fat_write(FatFileSystem *self, const char *filename, const void *in_buffer,
              uint32_t size) {
  char name[9] = {0};
  char ext[4] = {0};
  const char *dot = strchr(filename, '.');
  if (dot) {
    int name_len = dot - filename;
    if (name_len > 8)
      name_len = 8;
    strncpy(name, filename, name_len);
    strncpy(ext, dot + 1, 3);
  } else {
    strncpy(name, filename, 8);
  }

  serial_print("[fat/write] enter\n");
  int old_index = self->current_file_index;

  uint32_t entry_offset = find_free_dir_entry(self, self->pwd_cluster);
  if (entry_offset == 0) {
    serial_print("No free directory entry found\n");
    serial_print("[fat/write] no dir entry\n");
    self->current_file_index = old_index;
    return 0;
  }

  Fat16Entry new_entry;
  memset(&new_entry, 0, sizeof(Fat16Entry));

  memset(new_entry.filename, ' ', 8);
  memset(new_entry.ext, ' ', 3);
  for (int i = 0; i < 8 && name[i]; i++)
    new_entry.filename[i] = fat_to_upper(name[i]);
  for (int i = 0; i < 3 && ext[i]; i++)
    new_entry.ext[i] = fat_to_upper(ext[i]);

  new_entry.attributes = 0x20;

  uint16_t first_cluster = 0;
  uint16_t last_cluster = 0;
  uint32_t file_size = 0;

  uint32_t bytes_per_cluster = self->boot_sector.sectors_per_cluster * 512;

  uint8_t *cluster_buffer = (uint8_t *)allocate_memory(bytes_per_cluster);
  if (!cluster_buffer) {
    serial_print("[fat/write] alloc failed\n");
    return 0;
  }

  uint32_t data_start_sector =
      get_root_dir_start_sector(self->selected_partition_table,
                                &self->boot_sector) +
      (self->boot_sector.root_dir_entries * sizeof(Fat16Entry)) / 512;

  const uint8_t *input_cursor = (const uint8_t *)in_buffer;
  uint32_t bytes_remaining = size;

  serial_print("[fat/write] starting write loop\n");

  while (bytes_remaining > 0) {
    serial_print("[fat/write] write loop iteration\n");
    uint32_t bytes_to_stage = (bytes_remaining > bytes_per_cluster)
                                  ? bytes_per_cluster
                                  : bytes_remaining;
    memcpy(cluster_buffer, input_cursor, bytes_to_stage);

    serial_print("[fat/write] calling find_free_cluster\n");
    uint16_t new_cluster = find_free_cluster(self);
    serial_print("[fat/write] find_free_cluster done\n");
    if (new_cluster == 0) {
      serial_print("Disk full\n");
      serial_print("[fat/write] no cluster\n");
      break;
    }

    serial_print("[fat/write] calling set_fat_entry\n");
    set_fat_entry(self, new_cluster, 0xFFFF);
    serial_print("[fat/write] set_fat_entry done\n");

    if (first_cluster == 0) {
      first_cluster = new_cluster;
    } else {
      serial_print("[fat/write] linking clusters\n");
      set_fat_entry(self, last_cluster, new_cluster);
      serial_print("[fat/write] link done\n");
    }

    serial_print("[fat/write] writing cluster data\n");
    uint32_t cluster_sector =
        data_start_sector +
        (new_cluster - 2) * self->boot_sector.sectors_per_cluster;
    write_at_byte(cluster_sector * 512, cluster_buffer, bytes_to_stage);
    serial_print("[fat/write] cluster data written\n");

    last_cluster = new_cluster;
    file_size += bytes_to_stage;
    input_cursor += bytes_to_stage;
    bytes_remaining -= bytes_to_stage;

    serial_print("[fat/write] loop end\n");

    if (bytes_to_stage < bytes_per_cluster) {
      break;
    }
  }

  new_entry.starting_cluster = first_cluster;
  new_entry.file_size = file_size;

  serial_print("[fat/write] writing dir entry\n");
  write_at_byte(entry_offset, &new_entry, sizeof(Fat16Entry));
  serial_print("[fat/write] dir entry written\n");
  free_memory(cluster_buffer);

  self->current_file_index = old_index;
  serial_print("[fat/write] exit\n");

  return 1;
}
