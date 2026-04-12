typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

static inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port));
  return ret;
}

static uint8_t get_scancode() {
  if (inb(0x64) & 1)
    return inb(0x60);
  return 0;
}

__attribute__((section(".text.entry"))) void entry() {
  volatile char *video = (volatile char *)0xB8000;
  const char *msg = "Hello from FAT /games! Press ESC to return to CLI.";

  // Clear screen and print
  for (int i = 0; i < 80 * 25 * 2; i += 2) {
    video[i] = ' ';
    video[i + 1] = 0x07;
  }

  int j = 0;
  while (msg[j] != '\0') {
    video[j * 2] = msg[j];
    video[j * 2 + 1] = 0x0A; // Light green on black
    j++;
  }

  while (1) {
    uint8_t scancode = get_scancode();
    if (scancode == 0x01) {
      break;
    }
  }
}
