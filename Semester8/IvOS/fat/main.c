#include "fat.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  char name[8];
  char ext[3];
} FileName;

bool recursive_read(FatFileSystem *fs, FileName *path, unsigned int depth) {
  Fat16Entry current_entry;

  for (unsigned int i = 0; i < depth; i++) {
    if (!fat_find_file(fs, path[i].name, path[i].ext, &current_entry)) {
      return false;
    }

    if (i == depth - 1) {
      fat_read_file(fs, &current_entry, stderr);
      return true;
    }

    if (fat_change_dir(fs, path[i].name) == 0) {
      return false;
    }
  }
  return false;
}

int main(int argc, char **argv) {
  FILE *data = fopen("sd.img", "r+b");
  if (!data) {
    printf("Failed to open sd.img\n");
    return 1;
  }

  FatFileSystem fs;
  fat_init(&fs, data);

  int i;
  PartitionTable pt[4];

  fat_read_partitions(&fs, pt);
  fat_select_partition_table(&fs, &pt[0]);
  fat_read_boot_sector(&fs);

  if (argc > 1 && strcmp(argv[1], "print") == 0) {
    printf("Partition table\n-----------------------\n");
    for (i = 0; i < 4; i++) {
      printf("Partition %d, type %02X, ", i, pt[i].partition_type);
      printf("start sector %8d, length %8d sectors\n", pt[i].start_sector,
             pt[i].length_sectors);
    }

    printf("\nSeeking to first partition by %d sectors\n", pt[0].start_sector);

    printf("Volume_label %.11s, %d sectors size\n", fs.boot_sector.volume_label,
           fs.boot_sector.sector_size);

    printf("\nFilesystem root directory listing: count: "
           "%d\n-----------------------\n",
           fs.boot_sector.root_dir_entries);
    for (i = 0; i < fs.boot_sector.root_dir_entries; i++) {
      fat_read_directory_entry(&fs, 0);
      Fat16Entry entry = fs.current_file;

      /* Skip empty or deleted entries (0x00 = empty end of dir, 0xE5 =
       * arbitrarily deleted file byte) */
      if (entry.filename[0] != 0x00 &&
          (unsigned char)entry.filename[0] != 0xE5) {
        printf("%.8s.%.3s attributes 0x%02X starting cluster %8d len %8d B\n",
               entry.filename, entry.ext, entry.attributes,
               entry.starting_cluster, entry.file_size);
      }
    }

    FileName file_path[2] = {{.name = "ADR2    ", .ext = "   "},
                             {.name = "HISTORIE", .ext = "TXT"}};

    if (recursive_read(&fs, file_path, 2)) {
      fprintf(stdout, "\nRecursive read found file! Outputted to stderr.\n");
    } else {
      fprintf(stdout, "\nRecursive read failed to find the file!\n");
    }

    // Reset current dir
    fs.pwd_cluster = 0;

    printf("==== Root directory listing ====\n");
    fat_print_root_dir(&fs);

    printf("==== ADR1 directory listing ====\n");
    fat_change_dir(&fs, "ADR1");
    fat_print_dir(&fs, fs.pwd_cluster);

    Fat16Entry entry;
    if (fat_find_file(&fs, "VYUKA", "JPG", &entry)) {
      fat16entry_to_str(&entry);
    }

    fat_change_dir(&fs, "..");
    fat_change_dir(&fs, "ADR2");

    printf("==== ADR2 directory listing ====\n");
    fat_print_dir(&fs, fs.pwd_cluster);

    if (fat_find_file(&fs, "KOREN", "TXT", &entry)) {
      fat16entry_to_str(&entry);
    }
    printf("\n");
  } else if (argc > 1 && strcmp(argv[1], "write") == 0 && argc > 2) {
    printf("Testing write to %s...\n", argv[2]);
    if (fat_write(&fs, argv[2], stdin)) {
      printf("Write successful.\n");
    }
  } else if (argc > 1 && strcmp(argv[1], "delete") == 0 && argc > 2) {
    printf("Testing delete of %s...\n", argv[2]);
    if (fat_delete(&fs, argv[2])) {
      printf("Delete successful.\n");
    }
  } else if (argc > 1 && strcmp(argv[1], "ensure-dir") == 0 && argc > 2) {
    if (fat_mkdir(&fs, argv[2])) {
      printf("Directory ready: %s\n", argv[2]);
    } else {
      printf("Failed to ensure directory: %s\n", argv[2]);
      fclose(data);
      return 1;
    }
  } else if (argc > 1 && strcmp(argv[1], "write-in-dir") == 0 && argc > 3) {
    if (fat_write_in_dir(&fs, argv[2], argv[3], stdin)) {
      printf("Write successful: %s/%s\n", argv[2], argv[3]);
    } else {
      printf("Write failed: %s/%s\n", argv[2], argv[3]);
      fclose(data);
      return 1;
    }
  } else {
    printf("Usage: %s <print|write|delete|ensure-dir|write-in-dir> [args]\n",
           argv[0]);
  }

  fclose(data);
  return 0;
}
