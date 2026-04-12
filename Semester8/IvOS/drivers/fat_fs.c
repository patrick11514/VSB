#include "fat_fs.h"

#include "../drivers/serial.h"
#include "../lib/string.h"

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
  if (!self->selected_partition_table) {
    serial_print("No partition table selected\n");
    return;
  }

  self->current_file_index++;

  int entries_per_sector = 512 / sizeof(Fat16Entry);
  int entries_per_cluster =
      entries_per_sector * self->boot_sector.sectors_per_cluster;

  if (dir_cluster == 0) {
    int root_dir_start_sector = get_root_dir_start_sector(
        self->selected_partition_table, &self->boot_sector);
    self->current_entry_offset =
        root_dir_start_sector * 512 +
        (self->current_file_index * sizeof(Fat16Entry));

    read_at_byte(self->current_entry_offset, &self->current_file,
                 sizeof(Fat16Entry));
  } else {
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
  char target_name[8] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
  char target_ext[3] = {' ', ' ', ' '};

  for (int i = 0; i < 8 && name[i] != '\0'; i++)
    target_name[i] = name[i];
  for (int i = 0; i < 3 && ext[i] != '\0'; i++)
    target_ext[i] = ext[i];

  self->current_file_index = -1;

  int max_entries =
      (self->pwd_cluster == 0) ? self->boot_sector.root_dir_entries : 65535;

  for (int i = 0; i < max_entries; i++) {
    fat_read_directory_entry(self, self->pwd_cluster);
    Fat16Entry entry = self->current_file;

    if (entry.filename[0] == 0x00)
      break;

    if ((unsigned char)entry.filename[0] == 0xE5)
      continue;

    int match = 1;
    for (int j = 0; j < 8; j++)
      if (entry.filename[j] != target_name[j])
        match = 0;
    for (int j = 0; j < 3; j++)
      if (entry.ext[j] != target_ext[j])
        match = 0;

    if (match) {
      *out_entry = entry;
      return 1;
    }
  }
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
  for (uint16_t i = 2; i <= 0xFFEF; i++) {
    if (get_fat_entry(self, i) == 0x0000) {
      return i;
    }
  }
  return 0;
}

static uint32_t find_free_dir_entry(FatFileSystem *self, uint32_t dir_cluster) {
  int old_index = self->current_file_index;
  self->current_file_index = -1;

  int max_entries =
      (dir_cluster == 0) ? self->boot_sector.root_dir_entries : 65535;

  for (int i = 0; i < max_entries; i++) {
    fat_read_directory_entry(self, dir_cluster);

    if (self->current_file.filename[0] == 0x00 ||
        (unsigned char)self->current_file.filename[0] == 0xE5) {
      uint32_t offset = self->current_entry_offset;
      self->current_file_index = old_index;
      return offset;
    }
  }
  self->current_file_index = old_index;
  return 0;
}

int fat_delete(FatFileSystem *self, const char *filename) {
  Fat16Entry entry;
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

  if (!fat_find_file(self, name, ext, &entry)) {
    return 0;
  }

  uint32_t entry_offset = self->current_entry_offset;

  uint16_t cluster = entry.starting_cluster;
  while (cluster >= 2 && cluster <= 0xFFEF) {
    uint16_t next_cluster = get_fat_entry(self, cluster);
    set_fat_entry(self, cluster, 0x0000);
    cluster = next_cluster;
  }

  entry.filename[0] = 0xE5;
  write_at_byte(entry_offset, &entry, sizeof(Fat16Entry));

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

  uint32_t entry_offset = find_free_dir_entry(self, self->pwd_cluster);
  if (entry_offset == 0) {
    serial_print("No free directory entry found\n");
    return 0;
  }

  Fat16Entry new_entry;
  memset(&new_entry, 0, sizeof(Fat16Entry));

  memset(new_entry.filename, ' ', 8);
  memset(new_entry.ext, ' ', 3);
  for (int i = 0; i < 8 && name[i]; i++)
    new_entry.filename[i] = name[i];
  for (int i = 0; i < 3 && ext[i]; i++)
    new_entry.ext[i] = ext[i];

  new_entry.attributes = 0x20;

  uint16_t first_cluster = 0;
  uint16_t last_cluster = 0;
  uint32_t file_size = 0;

  uint32_t bytes_per_cluster = self->boot_sector.sectors_per_cluster * 512;

  fat_memory_reset();
  uint8_t *cluster_buffer = (uint8_t *)allocate_memory(bytes_per_cluster);
  if (!cluster_buffer) {
    return 0;
  }

  uint32_t data_start_sector =
      get_root_dir_start_sector(self->selected_partition_table,
                                &self->boot_sector) +
      (self->boot_sector.root_dir_entries * sizeof(Fat16Entry)) / 512;

  const uint8_t *input_cursor = (const uint8_t *)in_buffer;
  uint32_t bytes_remaining = size;

  while (bytes_remaining > 0) {
    uint32_t bytes_to_stage = (bytes_remaining > bytes_per_cluster)
                                  ? bytes_per_cluster
                                  : bytes_remaining;
    memcpy(cluster_buffer, input_cursor, bytes_to_stage);

    uint16_t new_cluster = find_free_cluster(self);
    if (new_cluster == 0) {
      serial_print("Disk full\n");
      break;
    }

    set_fat_entry(self, new_cluster, 0xFFFF);

    if (first_cluster == 0) {
      first_cluster = new_cluster;
    } else {
      set_fat_entry(self, last_cluster, new_cluster);
    }

    uint32_t cluster_sector =
        data_start_sector +
        (new_cluster - 2) * self->boot_sector.sectors_per_cluster;
    write_at_byte(cluster_sector * 512, cluster_buffer, bytes_to_stage);

    last_cluster = new_cluster;
    file_size += bytes_to_stage;
    input_cursor += bytes_to_stage;
    bytes_remaining -= bytes_to_stage;

    if (bytes_to_stage < bytes_per_cluster) {
      break;
    }
  }

  new_entry.starting_cluster = first_cluster;
  new_entry.file_size = file_size;

  write_at_byte(entry_offset, &new_entry, sizeof(Fat16Entry));

  return 1;
}
