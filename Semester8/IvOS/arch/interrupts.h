#ifndef ARCH_INTERRUPTS_H
#define ARCH_INTERRUPTS_H

#include <stdint.h>

void idt_set_gate(int n, uint32_t handler);
void idt_load(void);
void pic_remap(void);
void pic_send_eoi(int irq);
void timer_init(uint32_t frequency);
/* ISR stubs defined in assembly */
extern void isr_timer_stub(void);

#endif
