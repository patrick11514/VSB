#include "fat.h"
#include <stdbool.h>

typedef struct
{
    char name[8];
    char ext[3];
} FileName;

bool recursive_read(FatFileSystem *fs, FileName *path, unsigned int depth)
{
    Fat16Entry current_entry;

    for (unsigned int i = 0; i < depth; i++)
    {
        if (!fat_find_file(fs, path[i].name, path[i].ext, &current_entry))
        {
            return false;
        }

        if (i == depth - 1)
        {
            fat_read_file(fs, &current_entry, stderr);
            return true;
        }

        if (fat_change_dir(fs, path[i].name) == 0)
        {
            return false;
        }
    }
    return false;
}

int main(int argc, char **argv)
{
    FILE *data = fopen("sd.img", "rb");
    if (!data)
    {
        printf("Failed to open sd.img\n");
        return 1;
    }

    FatFileSystem fs;
    fat_init(&fs, data);

    int i;
    PartitionTable pt[4];

    fat_read_partitions(&fs, pt);

    printf("Partition table\n-----------------------\n");
    for (i = 0; i < 4; i++)
    {
        printf("Partition %d, type %02X, ", i, pt[i].partition_type);
        printf("start sector %8d, length %8d sectors\n", pt[i].start_sector, pt[i].length_sectors);
    }

    printf("\nSeeking to first partition by %d sectors\n", pt[0].start_sector);
    fat_select_partition_table(&fs, &pt[0]);
    fat_read_boot_sector(&fs);

    printf("Volume_label %.11s, %d sectors size\n", fs.boot_sector.volume_label, fs.boot_sector.sector_size);

    printf("\nFilesystem root directory listing: count: %d\n-----------------------\n", fs.boot_sector.root_dir_entries);
    for (i = 0; i < fs.boot_sector.root_dir_entries; i++)
    {
        fat_read_directory_entry(&fs, 0);
        Fat16Entry entry = fs.current_file;

        /* Skip empty or deleted entries (0x00 = empty end of dir, 0xE5 = arbitrarily deleted file byte) */
        if (entry.filename[0] != 0x00 && (unsigned char)entry.filename[0] != 0xE5)
        {
            printf("%.8s.%.3s attributes 0x%02X starting cluster %8d len %8d B\n", entry.filename, entry.ext, entry.attributes, entry.starting_cluster, entry.file_size);
        }
    }

    /*FileName file_path[2] = {
        { .name = "ADR1    ", .ext = "   " },
        { .name = "KOCKA   ", .ext = "JPG" }
    };*/

    FileName file_path[2] = {
        {.name = "ADR2    ", .ext = "   "},
        {.name = "HISTORIE", .ext = "TXT"}};

    if (recursive_read(&fs, file_path, 2))
    {
        fprintf(stdout, "\nRecursive read found file! Outputted to stderr.\n");
    }
    else
    {
        fprintf(stdout, "\nRecursive read failed to find the file!\n");
    }

    fat_print_tree(&fs);

    fclose(data);
    return 0;
}
