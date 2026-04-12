#ifndef FAT_FS_H
#define FAT_FS_H

#include "../lib/types.h"

#include "fat_io.h"

typedef struct FatFileSystem FatFileSystem;

struct FatFileSystem {
  PartitionTable *selected_partition_table;
  Fat16BootSector boot_sector;
  int current_file_index;
  Fat16Entry current_file;
  uint32_t current_entry_offset;
  uint16_t pwd_cluster;
};

void fat_init(FatFileSystem *fs);
void fat_read_partitions(FatFileSystem *self, PartitionTable *pt);
void fat_select_partition_table(FatFileSystem *self, PartitionTable *pt);
void fat_read_boot_sector(FatFileSystem *self);
void fat_read_directory_entry(FatFileSystem *self, uint32_t dir_cluster);
int fat_read_file(FatFileSystem *self, Fat16Entry *entry, void **out_buffer,
                  uint32_t *out_size);
int fat_find_file(FatFileSystem *self, const char *name, const char *ext,
                  Fat16Entry *out_entry);
int fat_change_dir(FatFileSystem *self, const char *dir_name);
void fat_print_dir(FatFileSystem *self, uint16_t start_cluster);
void fat_print_root_dir(FatFileSystem *self);
int fat_write(FatFileSystem *self, const char *filename, const void *in_buffer,
              uint32_t size);
int fat_delete(FatFileSystem *self, const char *filename);
int fat_delete_entry(FatFileSystem *self, const Fat16Entry *entry,
                     uint32_t entry_offset);
int fat_mkdir(FatFileSystem *self, const char *dirname);
int fat_rmdir(FatFileSystem *self, const char *dirname);

#endif
