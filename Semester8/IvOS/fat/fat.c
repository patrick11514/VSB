#include "fat.h"
#include <string.h>

void fat_read_partitions(FatFileSystem *self, PartitionTable *pt)
{
    fseek(self->data, 0x1BE, SEEK_SET); // Seek to start of partition table
    fread(pt, sizeof(PartitionTable), 4, self->data);
}

void fat_select_partition_table(FatFileSystem *self, PartitionTable *pt)
{
    self->selected_partition_table = pt;
}

void fat_read_boot_sector(FatFileSystem *self)
{
    if (!self->selected_partition_table)
    {
        printf("No partition table selected\n");
        return;
    }
    read_at_byte(self->data, self->selected_partition_table->start_sector * 512, &self->boot_sector, sizeof(Fat16BootSector));
}

static uint16_t get_fat_entry(FatFileSystem *self, uint16_t cluster)
{
    uint32_t fat_start_sector = get_fat_start_sector(self->selected_partition_table, &self->boot_sector, 0);
    uint32_t offset_bytes = cluster * 2;
    uint32_t sector_offset = offset_bytes / 512;
    uint32_t byte_in_sector = offset_bytes % 512;

    uint16_t next_cluster = 0;
    read_at_byte(self->data, (fat_start_sector + sector_offset) * 512 + byte_in_sector, &next_cluster, 2);

    return next_cluster;
}

void fat_read_directory_entry(FatFileSystem *self, uint32_t dir_cluster)
{
    if (!self->selected_partition_table)
    {
        printf("No partition table selected\n");
        return;
    }

    self->current_file_index++;

    int entries_per_sector = 512 / sizeof(Fat16Entry);
    int entries_per_cluster = entries_per_sector * self->boot_sector.sectors_per_cluster;

    if (dir_cluster == 0)
    {
        /* Root directory is mapped sequentially without a FAT cluster chain */
        int root_dir_start_sector = get_root_dir_start_sector(self->selected_partition_table, &self->boot_sector);
        read_at_byte(self->data, root_dir_start_sector * 512 + (self->current_file_index * sizeof(Fat16Entry)), &self->current_file, sizeof(Fat16Entry));
    }
    else
    {
        /* Subdirectories are stored as standard cluster chains; traverse FAT to the correct cluster */
        uint16_t current_cluster = dir_cluster;
        int local_index = self->current_file_index;

        while (local_index >= entries_per_cluster)
        {
            current_cluster = get_fat_entry(self, current_cluster);

            // end of cluster chain
            if (current_cluster >= 0xFFF8)
            {
                memset(&self->current_file, 0, sizeof(Fat16Entry));
                return;
            }
            local_index -= entries_per_cluster;
        }

        int data_sectors_start = get_root_dir_start_sector(self->selected_partition_table, &self->boot_sector) +
                                 (self->boot_sector.root_dir_entries * sizeof(Fat16Entry)) / 512;

        int cluster_sector = data_sectors_start + (current_cluster - 2) * self->boot_sector.sectors_per_cluster;
        read_at_byte(self->data, cluster_sector * 512 + (local_index * sizeof(Fat16Entry)), &self->current_file, sizeof(Fat16Entry));
    }
}

int fat_find_file(FatFileSystem *self, const char *name, const char *ext, Fat16Entry *out_entry)
{
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

    int max_entries = (self->pwd_cluster == 0) ? self->boot_sector.root_dir_entries : 65535;

    for (int i = 0; i < max_entries; i++)
    {
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

        if (match)
        {
            *out_entry = entry;
            return 1;
        }
    }
    return 0; // Not found
}

// TODO: need to change FILE* to something usefull in my OS in future
void fat_read_file(FatFileSystem *self, Fat16Entry *entry, FILE *out_stream)
{
    if (!entry || entry->starting_cluster < 2)
        return;

    uint16_t current_cluster = entry->starting_cluster;
    uint32_t remaining_bytes = entry->file_size;

    uint32_t data_start_sector = get_root_dir_start_sector(self->selected_partition_table, &self->boot_sector) +
                                 (self->boot_sector.root_dir_entries * sizeof(Fat16Entry)) / 512;

    uint8_t buffer[512];

    while (current_cluster >= 2 && current_cluster <= 0xFFEF && remaining_bytes > 0)
    {
        uint32_t cluster_sector = data_start_sector + (current_cluster - 2) * self->boot_sector.sectors_per_cluster;

        for (int i = 0; i < self->boot_sector.sectors_per_cluster && remaining_bytes > 0; i++)
        {
            read_at_byte(self->data, (cluster_sector + i) * 512, buffer, 512);

            uint32_t to_write = (remaining_bytes > 512) ? 512 : remaining_bytes;
            fwrite(buffer, 1, to_write, out_stream);
            remaining_bytes -= to_write;
        }

        current_cluster = get_fat_entry(self, current_cluster);
    }
}

int fat_change_dir(FatFileSystem *self, const char *dir_name)
{
    Fat16Entry entry;
    if (!fat_find_file(self, dir_name, "", &entry))
    {
        return 0;
    }

    if (!(entry.attributes & 0x10))
    {
        return 0;
    }

    self->pwd_cluster = entry.starting_cluster;
}

// Print-f only implementation (for now)
static void _print_tree(FatFileSystem *self, uint16_t start_cluster, uint8_t depth)
{
    int old_index = self->current_file_index;
    self->current_file_index = -1; // skip the fat name

    if (depth == 0)
    {
        printf("/\n");
    }

    while (1)
    {
        fat_read_directory_entry(self, start_cluster);
        Fat16Entry sub_entry = self->current_file;

        if (sub_entry.filename[0] == 0x00)
            break; // End of directory
        if (sub_entry.filename[0] == 0xE5)
            continue; // Deleted

        if (sub_entry.filename[0] == '.')
        {
            continue; // this should skip . && ..
        }

        for (uint8_t i = 0; i < depth; ++i)
        {
            printf("│   ");
        }

        printf("├── ");
        prety_print_name(sub_entry.filename, sub_entry.ext);

        if (sub_entry.attributes & 0x10)
        {
            printf("/\n");
            // sub-folder
            _print_tree(self, sub_entry.starting_cluster, depth + 1);
            continue;
        }

        printf("\n");
    }

    self->current_file_index = old_index;
}

void fat_print_tree(FatFileSystem *self)
{
    _print_tree(self, 0, 0);
}

void fat_init(FatFileSystem *fs, FILE *data)
{
    fs->data = data;
    fs->selected_partition_table = NULL;
    fs->current_file_index = -1;
    fs->pwd_cluster = 0;
}