#include "interrupts.h"
#include "io.h"
#include "../kernel/scheduler.h"
#include <stdint.h>

extern void isr_timer_stub(void);

/* C handler called from assembly stubs. irq_number is pushed on stack. */
void isr_handler_c(int irq_number) {
    if (irq_number == 0) {
        /* timer */
        scheduler_tick();
    }

    /* send EOI */
    if (irq_number >= 8) {
        /* slave */
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);
}
