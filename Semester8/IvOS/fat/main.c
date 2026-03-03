#include "fat.h"
#include <stdbool.h>

typedef struct {
    char name[8];
    char ext[3];
} FileName;

bool recursive_read(FatFileSystem *fs, FileName *path, unsigned int depth)
{
    uint32_t current_cluster = 0;
    Fat16Entry current_entry;

    for (unsigned int i = 0; i < depth; i++) {
        if (!fs->find_file(fs, current_cluster, path[i].name, path[i].ext, &current_entry)) {
            return false;
        }

        if (i == depth - 1) {
            fs->read_file(fs, &current_entry, stderr);
            return true;
        }

        if (!(current_entry.attributes & 0x10)) {
            /* Path component is not a directory */
            return false;
        }
        current_cluster = current_entry.starting_cluster;
    }
    return false;
}

void print_dir(FatFileSystem *fs, const char* name)
{
      Fat16Entry adr;
  if (fs->find_file(fs, 0, name, "", &adr))
  {
      printf("\nListing contents of %.8s.%.3s:\n-----------------------\n", name, "");
      fs->current_file_index = -1; // Reset index for directory reading
      while (1)
      {
          fs->read_directory_entry(fs, adr.starting_cluster);
          Fat16Entry sub_entry = fs->current_file;
          
          if (sub_entry.filename[0] == 0x00) break; // End of directory
          if (sub_entry.filename[0] == 0xE5) continue; // Deleted
          
          printf("%.8s.%.3s attributes 0x%02X starting cluster %8d len %8d B\n", 
                 sub_entry.filename, sub_entry.ext, sub_entry.attributes, 
                 sub_entry.starting_cluster, sub_entry.file_size);
      }
  }
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

  fs.read_partitions(&fs, pt);

  printf("Partition table\n-----------------------\n");
  for (i = 0; i < 4; i++)
  { 
    printf("Partition %d, type %02X, ", i, pt[i].partition_type);
    printf("start sector %8d, length %8d sectors\n", pt[i].start_sector, pt[i].length_sectors);
  }

  printf("\nSeeking to first partition by %d sectors\n", pt[0].start_sector);
  fs.select_partition_table(&fs, &pt[0]);
  fs.read_boot_sector(&fs);

  printf("Volume_label %.11s, %d sectors size\n", fs.boot_sector.volume_label, fs.boot_sector.sector_size);

  printf("\nFilesystem root directory listing: count: %d\n-----------------------\n", fs.boot_sector.root_dir_entries);
  for (i = 0; i < fs.boot_sector.root_dir_entries; i++)
  {
    fs.read_directory_entry(&fs, 0);
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
      { .name = "ADR2    ", .ext = "   " },
      { .name = "HISTORIE", .ext = "TXT" }
  };

  if (recursive_read(&fs, file_path, 2))
  {
      fprintf(stdout, "\nRecursive read found file! Outputted to stderr.\n");
  }
  else
  {
      fprintf(stdout, "\nRecursive read failed to find the file!\n");
  }

  print_dir(&fs, "ADR1");
  print_dir(&fs, "ADR2");


  fclose(data);
  return 0;
}
