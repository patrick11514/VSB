[bits 16]    ; 16 bit mode

switch_to_pm:
    cli                     ; 1. Disable interrupts

    lgdt [gdt_descriptor]   ; 2. Load the GDT

    mov eax, cr0            ; 3. Move the control register to eax
    or eax, 0x1             ; Set the first bit (Protection Enable)
    mov cr0, eax            ; Move it back to cr0

    jmp CODE_SEG:init_pm    ; 4. Far jump using our new code segment selector

[BITS 32]
init_pm:
    ; 5. Update segment registers
    mov ax, DATA_SEG        ; Load our data segment selector
    mov ds, ax              ; Data Segment
    mov ss, ax              ; Stack Segment
    mov es, ax              ; Extra Segment
    mov fs, ax
    mov gs, ax

    ; 6. Set up a new 32-bit stack
    mov ebp, 0x90000        ; Put the stack safely at the top of free conventional memory
    mov esp, ebp            ; Set the stack pointer

    extern kernel_main 
    call kernel_main        ; call the kernel.c main function

    jmp $                   ; Hang infinitely if the kernel returns

; -----------------------------------------------------------------------------
; Global Descriptor Table (GDT)
; -----------------------------------------------------------------------------
gdt_start:

gdt_null:                   ; The mandatory null descriptor
    dd 0x0
    dd 0x0

gdt_code:                   ; The code segment descriptor
    dw 0xffff               ; Limit (bits 0-15)
    dw 0x0                  ; Base (bits 0-15)
    db 0x0                  ; Base (bits 16-23)
    db 10011010b            ; Access byte (Present, Ring 0, Executable, Read)
    db 11001111b            ; Flags (32-bit, 4K granularity) + Limit (bits 16-19)
    db 0x0                  ; Base (bits 24-31)

gdt_data:                   ; The data segment descriptor
    dw 0xffff               ; Limit (bits 0-15)
    dw 0x0                  ; Base (bits 0-15)
    db 0x0                  ; Base (bits 16-23)
    db 10010010b            ; Access byte (Present, Ring 0, Read/Write)
    db 11001111b            ; Flags (32-bit, 4K granularity) + Limit (bits 16-19)
    db 0x0                  ; Base (bits 24-31)

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Size of our GDT
    dd gdt_start                ; Start address of our GDT

; Calculate the offsets for the segment selectors
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start