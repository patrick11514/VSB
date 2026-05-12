#include "../kernel/scheduler.h"
#include "interrupts.h"
#include "io.h"
#include <stdint.h>

extern void isr_timer_stub(void);

/* C handler called from assembly stubs. irq_number is pushed on stack. */
void isr_handler_c(int irq_number) {
  extern void serial_print(const char *);
  
  if (irq_number == 0) {
    /* timer */
    serial_print("[IRQ0]");
    /* send EOI before context switch so PIC is cleared */
    if (irq_number >= 8) {
      outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);
    scheduler_tick();
  }
}
