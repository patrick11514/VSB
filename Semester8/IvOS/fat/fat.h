#ifndef __FAT_H__
#define __FAT_H__ 1

#include "utils.h"

typedef struct FatFileSystem FatFileSystem;

struct FatFileSystem
{
    FILE *data;
    PartitionTable *selected_partition_table;
    Fat16BootSector boot_sector;
    int current_file_index;
    Fat16Entry current_file;
    uint32_t current_entry_offset;
    uint16_t pwd_cluster;
};

void fat_init(FatFileSystem *fs, FILE *data);
void fat_read_partitions(FatFileSystem *self, PartitionTable *pt);
void fat_select_partition_table(FatFileSystem *self, PartitionTable *pt);
void fat_read_boot_sector(FatFileSystem *self);
void fat_read_directory_entry(FatFileSystem *self, uint32_t dir_cluster);
void fat_read_file(FatFileSystem *self, Fat16Entry *entry, FILE *out_stream);
int fat_find_file(FatFileSystem *self, const char *name, const char *ext, Fat16Entry *out_entry);
int fat_change_dir(FatFileSystem *self, const char *dir_name);
void fat_print_dir(FatFileSystem *self, uint16_t start_cluster);
void fat_print_root_dir(FatFileSystem *self);
int fat_write(FatFileSystem *self, const char *filename, FILE *in_stream);
int fat_delete(FatFileSystem *self, const char *filename);

#endif