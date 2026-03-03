#ifndef __UTILS_H__
#define __UTILS_H__ 1

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "structs.h"

void read_at_byte(FILE *data, int byte, void *buffer, size_t size);
int ata_read_sector(FILE *data, uint32_t lba, uint8_t *buffer);
int get_fat_start_sector(PartitionTable *pt, Fat16BootSector *bs, int idx);
int get_root_dir_start_sector(PartitionTable *pt, Fat16BootSector *bs);

#endif