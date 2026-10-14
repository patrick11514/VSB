# IvOS - Implementation in Operating Systems

*Implementace v operačních systémech*

A 32-bit x86 Operating System kernel written from scratch in C and x86 Assembly.

## Components:
* **Bootloader:** x86 Real Mode bootloader (`boot.asm`)
* **Architecture:** Interrupt Descriptor Table (IDT), ISR handlers, Programmable Interrupt Controller (PIC), PIT Timer, Context switching
* **Drivers:** VGA text-mode driver, Serial COM driver, PS/2 Keyboard driver, IDE hard disk driver, FAT12/16/32 filesystem support
* **Kernel & Execution:** Cooperative multitasking scheduler, Green Threads (`gthreads`), interactive CLI shell
* **Apps & Games:** Userspace command-line utilities and games
