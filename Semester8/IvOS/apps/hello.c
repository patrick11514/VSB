__attribute__((section(".text.entry"))) void entry() {
  volatile char *video = (volatile char *)0xB8000;
  const char *msg = "Hello from dynamically loaded sector! System frozen.";

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
    // Freeze system
  }
}
