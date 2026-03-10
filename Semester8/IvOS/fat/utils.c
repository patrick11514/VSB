#include "utils.h"
#include <string.h>

static uint8_t sector_buffer[512];
static uint32_t cached_sector = 0xFFFFFFFF;

/*
Abstract something, we will then replace with stdlib or something like that
*/
void *allocate_memory(size_t size)
{
    return malloc(size);
}

void free_memory(void *ptr)
{
    free(ptr);
}

int ata_read_sector(FILE *data, uint32_t lba, uint8_t *buffer)
{
    fseek(data, lba * 512, SEEK_SET);
    size_t read = fread(buffer, 512, 1, data);
    return read == 1;
}

int ata_write_sector(FILE *data, uint32_t lba, const uint8_t *buffer)
{
    fseek(data, lba * 512, SEEK_SET);
    size_t written = fwrite(buffer, 512, 1, data);
    fflush(data);
    return written == 1;
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

void write_at_byte(FILE *data, int byte, const void *buffer, size_t size)
{
    uint32_t start_sector = byte / 512;
    uint32_t offset = byte % 512;
    const uint8_t *in = (const uint8_t *)buffer;
    size_t remaining = size;

    while (remaining > 0)
    {
        if (cached_sector != start_sector)
        {
            ata_read_sector(data, start_sector, sector_buffer); // Read-modify-write
            cached_sector = start_sector;
        }

        size_t available = 512 - offset;
        size_t to_copy = (remaining < available) ? remaining : available;
        memcpy(sector_buffer + offset, in, to_copy);

        ata_write_sector(data, start_sector, sector_buffer); // write back

        in += to_copy;
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

void fat16entry_to_str(Fat16Entry *entry)
{
    if (entry->attributes & 0x08)
    {
        printf("LABEL");
    }
    else
    {

        if (entry->attributes & 0x10)
        {
            putchar('D');
        }
        else
        {
            putchar('F');
        }

        putchar(' ');

        if (entry->attributes & 0x01)
        {
            printf("RO");
        }
        else
        {
            printf("RW");
        }
    }

    putchar(' ');

    prety_print_name((char *)entry->filename, (char *)entry->ext);

    putchar(' ');

    print_file_stat(entry, 1, 1);
}

void prety_print_name(char name[8], char ext[3])
{
    // print to name.ext if ext is empty, then just print name - skip empty chars in name and ext
    int has_ext = 0;
    for (int i = 0; i < 3; i++)
    {
        if (ext[i] != ' ')
        {
            has_ext = 1;
            break;
        }
    }

    for (int i = 0; i < 8; i++)
    {
        if (name[i] != ' ')
        {
            putchar(name[i]);
        }
    }

    if (has_ext)
    {
        putchar('.');
        for (int i = 0; i < 3; i++)
        {
            if (ext[i] != ' ')
            {
                putchar(ext[i]);
            }
        }
    }
}

void print_file_stat(Fat16Entry *entry, int time, int date)
{
    if (date)
    {
        int year = (entry->modify_date >> 9) + 1980;
        int month = (entry->modify_date >> 5) & 0b1111;
        int day = (entry->modify_date) & 0b11111;
        printf("%.4d.%.2d.%.2d", year, month, day);
    }

    if (time)
    {
        if (date)
        {
            putchar(' ');
        }
        int hours = (entry->modify_time >> 11);
        int minutes = ((entry->modify_time >> 5) & 0b111111);
        int seconds = (entry->modify_time & 0b11111) * 2;

        printf("%.2d:%.2d:%.2d", hours, minutes, seconds);
    }
}