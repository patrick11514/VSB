#include "interrupts.h"
#include "io.h"
#include <stdint.h>

/* PIT ports */
#define PIT_COMMAND 0x43
#define PIT_CHANNEL0 0x40
/* PIT base frequency */
#define PIT_FREQ 1193180

void timer_init(uint32_t frequency) {
  uint32_t divisor = PIT_FREQ / frequency;
  outb(PIT_COMMAND, 0x36); /* channel0, lobyte/hibyte, mode 3, binary */
  outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFF));
  outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8) & 0xFF));
}
