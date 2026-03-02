#include "fat.h"

static void _read_partitions(FatFileSystem *self, PartitionTable *pt)
{
    fseek(self->data, 0x1BE, SEEK_SET); // Seek to start of partition table
    fread(pt, sizeof(PartitionTable), 4, self->data);
}

static void _select_partition_table(FatFileSystem *self, PartitionTable *pt)
{
    self->selected_partition_table = pt;
}

static void _read_boot_sector(FatFileSystem *self)
{
    if (!self->selected_partition_table)
    {
        printf("No partition table selected\n");
        return;
    }
    read_at_byte(self->data, self->selected_partition_table->start_sector * 512, &self->boot_sector, sizeof(Fat16BootSector));
}

static void _read_directory_entry(FatFileSystem *self)
{
    if (!self->selected_partition_table)
    {
        printf("No partition table selected\n");
        return;
    }
    self->current_file_index++;
    int root_dir_start_sector = get_root_dir_start_sector(self->selected_partition_table, &self->boot_sector);
    read_at_byte(self->data, root_dir_start_sector + (self->current_file_index * sizeof(Fat16Entry)), &self->current_file, sizeof(Fat16Entry));
}

static void _read_file(FatFileSystem *self, void *buffer)
{
    if (self->current_file_index < 0)
    {
        printf("No file selected\n");
        return;
    }

    int ptr = 0;

    char *fat_tables[2];
    fat_tables[0] = get_fat_start_sector(self->selected_partition_table, &self->boot_sector, 0);
    fat_tables[1] = get_fat_start_sector(self->selected_partition_table, &self->boot_sector, 1);

    self->current_file
}

void fat_init(FatFileSystem *fs, FILE *data)
{
    fs->data = data;
    fs->selected_partition_table = NULL;
    fs->current_file_index = -1;

    fs->read_partitions = _read_partitions;
    fs->select_partition_table = _select_partition_table;
    fs->read_boot_sector = _read_boot_sector;
    fs->read_directory_entry = _read_directory_entry;
    fs->read_file = _read_file;
}

// TODO: READ FILE CONTENT
// TODO: READ SUB_DIRECTORIES