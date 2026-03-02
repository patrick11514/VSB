#include "fat.h"

int main()
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
  { // for all partition entries print basic info
    printf("Partition %d, type %02X, ", i, pt[i].partition_type);
    printf("start sector %8d, length %8d sectors\n", pt[i].start_sector, pt[i].length_sectors);
  }

  printf("\nSeeking to first partition by %d sectors\n", pt[0].start_sector);
  fs.select_partition_table(&fs, &pt[0]);
  fs.read_boot_sector(&fs);

  printf("Volume_label %.11s, %d sectors size\n", fs.boot_sector.volume_label, fs.boot_sector.sector_size);

  // Read all entries of root directory
  printf("\nFilesystem root directory listing: count: %d\n-----------------------\n", fs.boot_sector.root_dir_entries);
  for (i = 0; i < fs.boot_sector.root_dir_entries; i++)
  {
    fs.read_directory_entry(&fs);
    Fat16Entry entry = fs.current_file;
    // Skip if filename was never used, see http://www.tavi.co.uk/phobos/fat.html#file_attributes
    if (entry.filename[0] != 0x00)
    {
      printf("%.8s.%.3s attributes 0x%02X starting cluster %8d len %8d B\n", entry.filename, entry.ext, entry.attributes, entry.starting_cluster, entry.file_size);
    }
  }

  fclose(data);
  return 0;
}
