#include "../kernel/scheduler.h"
#include "interrupts.h"
#include "io.h"
#include <stdint.h>

extern void isr_timer_stub(void);
extern void keyboard_irq_handler(void); /* Add this! */

/* C handler called from assembly stubs. irq_number is pushed on stack. */
void isr_handler_c(int irq_number) {
  extern void serial_print(const char *);

  if (irq_number == 0) {
    /* timer */
    pic_send_eoi(0); // Use your nice helper function!
    scheduler_tick();
  } else if (irq_number == 1) {
    /* keyboard */
    keyboard_irq_handler(); // Call your ring buffer writer!
    pic_send_eoi(1);        // VERY IMPORTANT: Acknowledge the keyboard
  }
}