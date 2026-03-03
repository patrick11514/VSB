#include "utils.h"
#include <string.h>

static uint8_t sector_buffer[512];
static uint32_t cached_sector = 0xFFFFFFFF;

/*
Abstract something, we will then replace with stdlib or something like that
*/
int ata_read_sector(FILE *data, uint32_t lba, uint8_t *buffer)
{
    fseek(data, lba * 512, SEEK_SET);
    size_t read = fread(buffer, 512, 1, data);
    return read == 1;
}


void read_at_byte(FILE *data, int byte, void *buffer, size_t size)
{
    uint32_t start_sector = byte / 512;
    uint32_t offset = byte % 512;
    uint8_t *out = (uint8_t *)buffer;
    size_t remaining = size;

    while (remaining > 0)
    {
        if (cached_sector != start_sector)
        {
            ata_read_sector(data, start_sector, sector_buffer);
            cached_sector = start_sector;
        }

        size_t available = 512 - offset;
        size_t to_copy = (remaining < available) ? remaining : available;
        memcpy(out, sector_buffer + offset, to_copy);

        out += to_copy;
        remaining -= to_copy;
        start_sector++;
        offset = 0;
    }
}

int get_fat_start_sector(PartitionTable *pt, Fat16BootSector *bs, int idx)
{
    return pt->start_sector + bs->reserved_sectors + idx * bs->fat_size_sectors;
}

int get_root_dir_start_sector(PartitionTable *pt, Fat16BootSector *bs)
{
    return get_fat_start_sector(pt, bs, bs->number_of_fats);
}