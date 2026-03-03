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

    void (*read_partitions)(FatFileSystem *self, PartitionTable *pt);
    void (*select_partition_table)(FatFileSystem *self, PartitionTable *pt);
    void (*read_boot_sector)(FatFileSystem *self);
    void (*read_directory_entry)(FatFileSystem *self, uint32_t dir_cluster);
    void (*read_file)(FatFileSystem *self, Fat16Entry *entry, FILE *out_stream);
    int (*find_file)(FatFileSystem *self, uint32_t dir_cluster, const char *name, const char *ext, Fat16Entry *out_entry);
};

void fat_init(FatFileSystem *fs, FILE *data);

#endif