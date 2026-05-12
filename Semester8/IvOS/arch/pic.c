#include "interrupts.h"
#include "io.h"
#include <stdint.h>

#define PIC1_CMD 0x20
#define PIC1_DATA 0x21
#define PIC2_CMD 0xA0
#define PIC2_DATA 0xA1

void pic_remap(void) {
    /* Initialization Control Word 1 */
    outb(PIC1_CMD, 0x11);
    outb(PIC2_CMD, 0x11);
    /* ICW2: vector offset */
    outb(PIC1_DATA, 0x20); /* Master offset 0x20 */
    outb(PIC2_DATA, 0x28); /* Slave offset 0x28 */
    /* ICW3: tell master/slave about each other */
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    /* ICW4 */
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
    /* Mask none (enable all) */
    outb(PIC1_DATA, 0x0);
    outb(PIC2_DATA, 0x0);
}

void pic_send_eoi(int irq) {
    if (irq >= 8)
        outb(PIC2_CMD, 0x20);
    outb(PIC1_CMD, 0x20);
}
