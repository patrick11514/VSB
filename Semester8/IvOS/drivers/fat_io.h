#ifndef FAT_IO_H
#define FAT_IO_H

#include "../lib/types.h"

#include "fat_structs.h"

#define FAT_GLOBAL_BUFFER_SIZE (2U * 1024U * 1024U)

void fat_memory_reset(void);
void *fat_global_buffer_ptr(void);

void *allocate_memory(size_t size);
void free_memory(void *ptr);

int ata_read_sector(uint32_t lba, uint8_t *buffer);
int ata_write_sector(uint32_t lba, const uint8_t *buffer);

void read_at_byte(uint32_t byte, void *buffer, size_t size);
void write_at_byte(uint32_t byte, const void *buffer, size_t size);

int get_fat_start_sector(PartitionTable *pt, Fat16BootSector *bs, int idx);
int get_root_dir_start_sector(PartitionTable *pt, Fat16BootSector *bs);

void fat16entry_to_str(Fat16Entry *entry);
void prety_print_name(char name[8], char ext[3]);
void print_file_stat(Fat16Entry *entry, int time, int date);

#endif
