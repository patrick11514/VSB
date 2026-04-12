#include "fat.h"
#include <string.h>

void fat_read_partitions(FatFileSystem *self, PartitionTable *pt) {
  fseek(self->data, 0x1BE, SEEK_SET); // Seek to start of partition table
  fread(pt, sizeof(PartitionTable), 4, self->data);
}

void fat_select_partition_table(FatFileSystem *self, PartitionTable *pt) {
  self->selected_partition_table = pt;
}

void fat_read_boot_sector(FatFileSystem *self) {
  if (!self->selected_partition_table) {
    printf("No partition table selected\n");
    return;
  }
  read_at_byte(self->data, self->selected_partition_table->start_sector * 512,
               &self->boot_sector, sizeof(Fat16BootSector));
}

static uint16_t get_fat_entry(FatFileSystem *self, uint16_t cluster) {
  uint32_t fat_start_sector = get_fat_start_sector(
      self->selected_partition_table, &self->boot_sector, 0);
  uint32_t offset_bytes = cluster * 2;
  uint32_t sector_offset = offset_bytes / 512;
  uint32_t byte_in_sector = offset_bytes % 512;

  uint16_t next_cluster = 0;
  read_at_byte(self->data,
               (fat_start_sector + sector_offset) * 512 + byte_in_sector,
               &next_cluster, 2);

  return next_cluster;
}

void fat_read_directory_entry(FatFileSystem *self, uint32_t dir_cluster) {
  if (!self->selected_partition_table) {
    printf("No partition table selected\n");
    return;
  }

  self->current_file_index++;

  int entries_per_sector = 512 / sizeof(Fat16Entry);
  int entries_per_cluster =
      entries_per_sector * self->boot_sector.sectors_per_cluster;

  if (dir_cluster == 0) {
    /* Root directory is mapped sequentially without a FAT cluster chain */
    int root_dir_start_sector = get_root_dir_start_sector(
        self->selected_partition_table, &self->boot_sector);
    self->current_entry_offset =
        root_dir_start_sector * 512 +
        (self->current_file_index * sizeof(Fat16Entry));
    read_at_byte(self->data, self->current_entry_offset, &self->current_file,
                 sizeof(Fat16Entry));
  } else {
    /* Subdirectories are stored as standard cluster chains; traverse FAT to the
     * correct cluster */
    uint16_t current_cluster = dir_cluster;
    int local_index = self->current_file_index;

    while (local_index >= entries_per_cluster) {
      current_cluster = get_fat_entry(self, current_cluster);

      // end of cluster chain
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
    read_at_byte(self->data, self->current_entry_offset, &self->current_file,
                 sizeof(Fat16Entry));
  }
}

int fat_find_file(FatFileSystem *self, const char *name, const char *ext,
                  Fat16Entry *out_entry) {
  /*
   * Format target name and extension to match FAT 8.3 convention:
   * 8 bytes for name, 3 for extension, space-padded, without null-terminators.
   */
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
      break; /* Reached empty trailing entries in directory block */

    if ((unsigned char)entry.filename[0] == 0xE5)
      continue; /* Skip deleted files */

    /* Compare the constructed 8.3 filename bounds manually */
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
  return 0; // Not found
}

// TODO: need to change FILE* to something usefull in my OS in future
void fat_read_file(FatFileSystem *self, Fat16Entry *entry, FILE *out_stream) {
  if (!entry || entry->starting_cluster < 2)
    return;

  uint16_t current_cluster = entry->starting_cluster;
  uint32_t remaining_bytes = entry->file_size;

  uint32_t data_start_sector =
      get_root_dir_start_sector(self->selected_partition_table,
                                &self->boot_sector) +
      (self->boot_sector.root_dir_entries * sizeof(Fat16Entry)) / 512;

  uint8_t buffer[512];

  while (current_cluster >= 2 && current_cluster <= 0xFFEF &&
         remaining_bytes > 0) {
    uint32_t cluster_sector =
        data_start_sector +
        (current_cluster - 2) * self->boot_sector.sectors_per_cluster;

    for (int i = 0;
         i < self->boot_sector.sectors_per_cluster && remaining_bytes > 0;
         i++) {
      read_at_byte(self->data, (cluster_sector + i) * 512, buffer, 512);

      uint32_t to_write = (remaining_bytes > 512) ? 512 : remaining_bytes;
      fwrite(buffer, 1, to_write, out_stream);
      remaining_bytes -= to_write;
    }

    current_cluster = get_fat_entry(self, current_cluster);
  }
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

// Print-f only implementation (for now)
void _print_dir(FatFileSystem *self, uint16_t start_cluster, uint8_t depth,
                int classic) {
  int old_index = self->current_file_index;
  self->current_file_index = -1; // skip the fat name

  if (depth == 0) {
    printf("/\n");
  }

  while (1) {
    fat_read_directory_entry(self, start_cluster);
    Fat16Entry sub_entry = self->current_file;

    if (sub_entry.filename[0] == 0x00)
      break; // End of directory
    if (sub_entry.filename[0] == 0xE5)
      continue; // Deleted

    if (sub_entry.filename[0] == '.') {
      continue; // this should skip . && ..
    }

    for (uint8_t i = 0; i < depth; ++i) {
      printf("│   ");
    }

    printf("├── ");

    if (classic) {
      prety_print_name((char *)sub_entry.filename, (char *)sub_entry.ext);
    } else {
      fat16entry_to_str(&sub_entry);
      putchar('\n');
    }

    if (sub_entry.attributes & 0x10) {
      if (classic) {
        printf("/\n");
      }
      // sub-folder
      _print_dir(self, sub_entry.starting_cluster, depth + 1, classic);
      continue;
    }

    if (classic) {
      putchar(' ');
      print_file_stat(&sub_entry, 1, 1);
      putchar('\n');
    }
  }

  self->current_file_index = old_index;
}

void fat_print_root_dir(FatFileSystem *self) { _print_dir(self, 0, 0, 1); }

void fat_print_dir(FatFileSystem *self, uint16_t start_cluster) {
  _print_dir(self, start_cluster, 0, 1);
}

void fat_init(FatFileSystem *fs, FILE *data) {
  fs->data = data;
  fs->selected_partition_table = NULL;
  fs->current_file_index = -1;
  fs->pwd_cluster = 0;
}

static void set_fat_entry(FatFileSystem *self, uint16_t cluster,
                          uint16_t value) {
  for (int i = 0; i < self->boot_sector.number_of_fats; i++) {
    uint32_t fat_start_sector = get_fat_start_sector(
        self->selected_partition_table, &self->boot_sector, i);
    uint32_t offset_bytes = cluster * 2;
    write_at_byte(self->data, fat_start_sector * 512 + offset_bytes, &value, 2);
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
    return 0; // file not found
  }

  uint32_t entry_offset = self->current_entry_offset;

  // mark clusters as free
  uint16_t cluster = entry.starting_cluster;
  while (cluster >= 2 && cluster <= 0xFFEF) {
    uint16_t next_cluster = get_fat_entry(self, cluster);
    set_fat_entry(self, cluster, 0x0000); // mark free
    cluster = next_cluster;
  }

  // mark dir entry deleted
  entry.filename[0] = 0xE5;
  write_at_byte(self->data, entry_offset, &entry, sizeof(Fat16Entry));

  return 1;
}

int fat_write(FatFileSystem *self, const char *filename, FILE *in_stream) {
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
    printf("No free directory entry found\n");
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

  new_entry.attributes = 0x20; // Archive

  uint16_t first_cluster = 0;
  uint16_t last_cluster = 0;
  uint32_t file_size = 0;

  // calculate bytes per cluster
  uint32_t bytes_per_cluster = self->boot_sector.sectors_per_cluster * 512;
  uint8_t *cluster_buffer = allocate_memory(bytes_per_cluster);
  if (!cluster_buffer)
    return 0;

  uint32_t data_start_sector =
      get_root_dir_start_sector(self->selected_partition_table,
                                &self->boot_sector) +
      (self->boot_sector.root_dir_entries * sizeof(Fat16Entry)) / 512;

  size_t bytes_read;
  while ((bytes_read = fread(cluster_buffer, 1, bytes_per_cluster, in_stream)) >
         0) {
    uint16_t new_cluster = find_free_cluster(self);
    if (new_cluster == 0) {
      printf("Disk full\n");
      break;
    }

    set_fat_entry(self, new_cluster, 0xFFFF); // Mark EOC

    if (first_cluster == 0) {
      first_cluster = new_cluster;
    } else {
      set_fat_entry(self, last_cluster, new_cluster); // Link
    }

    // write data to cluster
    uint32_t cluster_sector =
        data_start_sector +
        (new_cluster - 2) * self->boot_sector.sectors_per_cluster;
    write_at_byte(self->data, cluster_sector * 512, cluster_buffer,
                  bytes_read); // write only the file data read; we can leave
                               // the rest unchanged or zero it out.

    last_cluster = new_cluster;
    file_size += bytes_read;

    if (bytes_read < bytes_per_cluster) {
      break; // EOF reached during fread if less bytes than chunk read
    }
  }

  free_memory(cluster_buffer);

  new_entry.starting_cluster = first_cluster;
  new_entry.file_size = file_size;

  write_at_byte(self->data, entry_offset, &new_entry, sizeof(Fat16Entry));

  return 1;
}

int fat_mkdir(FatFileSystem *self, const char *dirname) {
  if (!dirname || dirname[0] == '\0') {
    return 0;
  }

  Fat16Entry existing;
  if (fat_find_file(self, dirname, "", &existing)) {
    return (existing.attributes & 0x10) != 0;
  }

  uint32_t entry_offset = find_free_dir_entry(self, self->pwd_cluster);
  if (entry_offset == 0) {
    printf("No free directory entry found\n");
    return 0;
  }

  uint16_t cluster = find_free_cluster(self);
  if (cluster == 0) {
    printf("Disk full\n");
    return 0;
  }

  set_fat_entry(self, cluster, 0xFFFF);

  uint32_t bytes_per_cluster = self->boot_sector.sectors_per_cluster * 512;
  uint8_t *cluster_buffer = allocate_memory(bytes_per_cluster);
  if (!cluster_buffer) {
    return 0;
  }

  memset(cluster_buffer, 0, bytes_per_cluster);

  uint32_t data_start_sector =
      get_root_dir_start_sector(self->selected_partition_table,
                                &self->boot_sector) +
      (self->boot_sector.root_dir_entries * sizeof(Fat16Entry)) / 512;
  uint32_t cluster_sector =
      data_start_sector + (cluster - 2) * self->boot_sector.sectors_per_cluster;
  write_at_byte(self->data, cluster_sector * 512, cluster_buffer,
                bytes_per_cluster);
  free_memory(cluster_buffer);

  Fat16Entry dir_entry;
  memset(&dir_entry, 0, sizeof(Fat16Entry));
  memset(dir_entry.filename, ' ', 8);
  memset(dir_entry.ext, ' ', 3);

  for (int i = 0; i < 8 && dirname[i]; i++) {
    dir_entry.filename[i] = dirname[i];
  }

  dir_entry.attributes = 0x10;
  dir_entry.starting_cluster = cluster;
  dir_entry.file_size = 0;

  write_at_byte(self->data, entry_offset, &dir_entry, sizeof(Fat16Entry));
  return 1;
}

int fat_write_in_dir(FatFileSystem *self, const char *dirname,
                     const char *filename, FILE *in_stream) {
  if (!self || !dirname || !filename || !in_stream) {
    return 0;
  }

  uint16_t old_pwd = self->pwd_cluster;

  if (!fat_change_dir(self, dirname)) {
    self->pwd_cluster = old_pwd;
    return 0;
  }

  fat_delete(self, filename);

  int ok = fat_write(self, filename, in_stream);
  self->pwd_cluster = old_pwd;
  return ok;
}