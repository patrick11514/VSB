#include "ide.h"
#include "../arch/io.h"

// Basic ATA/IDE PIO Driver
// Hardcoded to primary bus (0x1F0) and master drive (0xA0)

#define ATA_DATA 0x1F0
#define ATA_ERROR 0x1F1
#define ATA_SECTOR_COUNT 0x1F2
#define ATA_LBA_LO 0x1F3
#define ATA_LBA_MID 0x1F4
#define ATA_LBA_HI 0x1F5
#define ATA_DRIVE_HEAD 0x1F6
#define ATA_STATUS_CMD 0x1F7

#define ATA_CMD_READ_PIO 0x20
#define ATA_CMD_WRITE_PIO 0x30

static void ide_wait() {
  for (int i = 0; i < 4; i++) {
    inb(ATA_STATUS_CMD + 0x200); // 0x3F6 (Alternate Status)
  }
}

static void ide_polling() {
  uint8_t status;
  while (((status = inb(ATA_STATUS_CMD)) & 0x80)) {
  } // Wait while BSY
  while (!(status & 0x08) && !(status & 0x01)) {
  } // Wait until DRQ or ERR
}

int ide_read_sector(uint32_t lba, void *buffer) {
  outb(ATA_DRIVE_HEAD,
       0xE0 | ((lba >> 24) &
               0x0F)); // Select master drive, LBA mode, highest 4 bits of LBA
  outb(ATA_SECTOR_COUNT, 1); // Read one sector
  outb(ATA_LBA_LO, (uint8_t)lba);
  outb(ATA_LBA_MID, (uint8_t)(lba >> 8));
  outb(ATA_LBA_HI, (uint8_t)(lba >> 16));
  outb(ATA_STATUS_CMD, ATA_CMD_READ_PIO); // Send read command

  ide_polling();

  uint16_t *buf = (uint16_t *)buffer;
  for (int i = 0; i < 256; i++) {
    buf[i] = inw(ATA_DATA);
  }

  ide_wait();
  return 0;
}

int ide_write_sector(uint32_t lba, const void *buffer) {
  outb(ATA_DRIVE_HEAD,
       0xE0 | ((lba >> 24) &
               0x0F)); // Select master drive, LBA mode, highest 4 bits of LBA
  outb(ATA_SECTOR_COUNT, 1); // Write one sector
  outb(ATA_LBA_LO, (uint8_t)lba);
  outb(ATA_LBA_MID, (uint8_t)(lba >> 8));
  outb(ATA_LBA_HI, (uint8_t)(lba >> 16));
  outb(ATA_STATUS_CMD, ATA_CMD_WRITE_PIO); // Send write command

  ide_polling();

  const uint16_t *buf = (const uint16_t *)buffer;
  for (int i = 0; i < 256; i++) {
    outw(ATA_DATA, buf[i]);
  }

  // Some drives need you to flush cache after write, but leaving it basic for
  // now
  ide_wait();
  return 0;
}
