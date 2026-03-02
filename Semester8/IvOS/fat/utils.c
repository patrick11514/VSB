#include "utils.h"

void seek_by_bytes(FILE *data, int bytes)
{
    fseek(data, bytes, SEEK_SET);
}

void read_at_byte(FILE *data, int byte, void *buffer, size_t size)
{
    seek_by_bytes(data, byte);
    fread(buffer, size, 1, data);
}

int get_fat_start_sector(PartitionTable *pt, Fat16BootSector *bs, int idx)
{
    return pt->start_sector * 512 + (bs->reserved_sectors + idx * bs->fat_size_sectors) * bs->sector_size;
}

int get_root_dir_start_sector(PartitionTable *pt, Fat16BootSector *bs)
{
    return get_fat_start_sector(pt, bs, bs->number_of_fats);
}