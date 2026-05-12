#include "../lib/api.h"

__attribute__((section(".text.entry"))) void entry(os_api_t *api) {

  api->serial_print("Hello from FAT /games! Press ESC to return to CLI.\n");
  const char *msg = "Hello from FAT /games! Press ESC to return to CLI.\n";
  api->print(msg);

  while (1) {
    uint8_t scancode = api->getchar();

    if (scancode == 27 /* ESC */) {
      break;
    }
  }
}
