#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct AppHeader {
  uint32_t magic;
  char name[16];
  uint32_t start_lba;
  uint32_t load_addr;
  uint32_t size_sectors;
} __attribute__((packed));

int main(int argc, char **argv) {
  if (argc < 6) {
    printf(
        "Usage: %s <out_file> <name> <start_lba> <load_addr> <size_sectors>\n",
        argv[0]);
    return 1;
  }

  struct AppHeader hdr;
  memset(&hdr, 0, sizeof(hdr));
  hdr.magic = 0xDEADBEEF;
  strncpy(hdr.name, argv[2], 15);
  sscanf(argv[3], "%u", &hdr.start_lba);
  sscanf(argv[4], "%x", &hdr.load_addr);
  sscanf(argv[5], "%u", &hdr.size_sectors);

  uint8_t sector[512];
  memset(sector, 0, sizeof(sector));
  memcpy(sector, &hdr, sizeof(hdr));

  FILE *f = fopen(argv[1], "wb");
  if (!f)
    return 1;
  fwrite(sector, 1, 512, f);
  fclose(f);

  return 0;
}
