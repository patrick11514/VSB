#include "interrupts.h"
#include "io.h"
#include <stdint.h>

struct idt_entry {
  uint16_t base_lo;
  uint16_t sel;
  uint8_t always0;
  uint8_t flags;
  uint16_t base_hi;
} __attribute__((packed));

struct idtr {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

static struct idt_entry idt[256];
static struct idtr idt_pointer;

void idt_set_gate(int n, uint32_t handler) {
  idt[n].base_lo = (uint16_t)(handler & 0xFFFF);
  idt[n].sel = 0x08; /* kernel code segment */
  idt[n].always0 = 0;
  idt[n].flags = 0x8E; /* interrupt gate */
  idt[n].base_hi = (uint16_t)((handler >> 16) & 0xFFFF);
}

void idt_load(void) {
  idt_pointer.limit = (uint16_t)(sizeof(idt) - 1);
  idt_pointer.base = (uint32_t)&idt;
  /* lidt */
  __asm__ volatile("lidt (%0)" : : "r"(&idt_pointer));
}
