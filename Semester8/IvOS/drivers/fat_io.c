#include "fat_io.h"

#include "../drivers/serial.h"
#include "../lib/string.h"
#include "ide.h"

static uint8_t sector_buffer[512];
static uint32_t cached_sector = 0xFFFFFFFF;

static uint8_t fat_global_buffer[FAT_GLOBAL_BUFFER_SIZE];
static size_t fat_global_buffer_offset = 0;

static void serial_write_padded_u32(unsigned int value, unsigned int width) {
  char buffer[10];
  size_t index = 0;

  while (value > 0) {
    buffer[index++] = (char)('0' + (value % 10));
    value /= 10;
  }

  while (index < width) {
    buffer[index++] = '0';
  }

  while (index > 0) {
    serial_putchar(buffer[--index]);
  }
}

void fat_memory_reset(void) { fat_global_buffer_offset = 0; }

void *fat_global_buffer_ptr(void) { return fat_global_buffer; }

void *allocate_memory(size_t size) {
  if (size == 0) {
    return NULL;
  }

  if (fat_global_buffer_offset + size > FAT_GLOBAL_BUFFER_SIZE) {
    return NULL;
  }

  void *ptr = &fat_global_buffer[fat_global_buffer_offset];
  fat_global_buffer_offset += size;
  return ptr;
}

void free_memory(void *ptr) { (void)ptr; }

int ata_read_sector(uint32_t lba, uint8_t *buffer) {
  return ide_read_sector(lba, buffer) == 0;
}

int ata_write_sector(uint32_t lba, const uint8_t *buffer) {
  return ide_write_sector(lba, buffer) == 0;
}

void read_at_byte(uint32_t byte, void *buffer, size_t size) {
  uint32_t start_sector = byte / 512;
  uint32_t offset = byte % 512;
  uint8_t *out = (uint8_t *)buffer;
  size_t remaining = size;

  while (remaining > 0) {
    if (cached_sector != start_sector) {
      ata_read_sector(start_sector, sector_buffer);
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

void write_at_byte(uint32_t byte, const void *buffer, size_t size) {
  serial_print("[fat_io/write_at_byte] enter\n");
  uint32_t start_sector = byte / 512;
  uint32_t offset = byte % 512;
  const uint8_t *in = (const uint8_t *)buffer;
  size_t remaining = size;

  while (remaining > 0) {
    serial_print("[fat_io/write_at_byte] loop start\n");
    if (cached_sector != start_sector) {
      serial_print("[fat_io/write_at_byte] read sector\n");
      ata_read_sector(start_sector, sector_buffer); // Read-modify-write
      cached_sector = start_sector;
      serial_print("[fat_io/write_at_byte] read done\n");
    }

    size_t available = 512 - offset;
    size_t to_copy = (remaining < available) ? remaining : available;
    serial_print("[fat_io/write_at_byte] memcpy\n");
    memcpy(sector_buffer + offset, in, to_copy);

    serial_print("[fat_io/write_at_byte] write sector\n");
    ata_write_sector(start_sector, sector_buffer); // write back
    serial_print("[fat_io/write_at_byte] write done\n");

    in += to_copy;
    remaining -= to_copy;
    start_sector++;
    offset = 0;
  }
  serial_print("[fat_io/write_at_byte] exit\n");
}

int get_fat_start_sector(PartitionTable *pt, Fat16BootSector *bs, int idx) {
  return pt->start_sector + bs->reserved_sectors + idx * bs->fat_size_sectors;
}

int get_root_dir_start_sector(PartitionTable *pt, Fat16BootSector *bs) {
  return get_fat_start_sector(pt, bs, bs->number_of_fats);
}

void fat16entry_to_str(Fat16Entry *entry) {
  if (entry->attributes & 0x08) {
    serial_print("LABEL");
  } else {
    if (entry->attributes & 0x10) {
      serial_putchar('D');
    } else {
      serial_putchar('F');
    }

    serial_putchar(' ');

    if (entry->attributes & 0x01) {
      serial_print("RO");
    } else {
      serial_print("RW");
    }
  }

  serial_putchar(' ');

  prety_print_name((char *)entry->filename, (char *)entry->ext);

  serial_putchar(' ');

  print_file_stat(entry, 1, 1);
}

void prety_print_name(char name[8], char ext[3]) {
  int has_ext = 0;
  for (int i = 0; i < 3; i++) {
    if (ext[i] != ' ') {
      has_ext = 1;
      break;
    }
  }

  for (int i = 0; i < 8; i++) {
    if (name[i] != ' ') {
      serial_putchar(name[i]);
    }
  }

  if (has_ext) {
    serial_putchar('.');
    for (int i = 0; i < 3; i++) {
      if (ext[i] != ' ') {
        serial_putchar(ext[i]);
      }
    }
  }
}

void print_file_stat(Fat16Entry *entry, int time, int date) {
  if (date) {
    int year = (entry->modify_date >> 9) + 1980;
    int month = (entry->modify_date >> 5) & 0b1111;
    int day = (entry->modify_date) & 0b11111;
    serial_write_padded_u32((unsigned int)year, 4);
    serial_putchar('.');
    serial_write_padded_u32((unsigned int)month, 2);
    serial_putchar('.');
    serial_write_padded_u32((unsigned int)day, 2);
  }

  if (time) {
    if (date) {
      serial_putchar(' ');
    }
    int hours = (entry->modify_time >> 11);
    int minutes = ((entry->modify_time >> 5) & 0b111111);
    int seconds = (entry->modify_time & 0b11111) * 2;

    serial_write_padded_u32((unsigned int)hours, 2);
    serial_putchar(':');
    serial_write_padded_u32((unsigned int)minutes, 2);
    serial_putchar(':');
    serial_write_padded_u32((unsigned int)seconds, 2);
  }
}
