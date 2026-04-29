[bits 16]    ; 16 bit mode
[org 0x7c00] ; starting addr

global _start
_start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti

boot_message:
    mov si, msg
    mov bp, msg
    
    mov ah, 0x03
    int 0x10

    mov bx, 0x02
    mov ah, 0x13 
    mov al, 0x01
    mov cx, 16
    int 0x10

load_sector:
    mov ah, 0x42    ; BIOS Extended Read Sector function
    mov dl, 0x80    ; Drive number (0x80 for first HDD. Usually best to use the DL value passed by BIOS at boot)
    mov si, dap     ; Point SI to our Disk Address Packet
    
    int 0x13        ; Call BIOS disk interrupt
    jc disk_error   ; Jump if carry flag is set (error occurred)

    jmp 0x0000:0x7e00 ; Jump to the newly loaded code

dap:
    db 0x10         ; Size of DAP (16 bytes)
    db 0            ; Reserved (always 0)
    dw 80           ; Number of sectors to read
    dw 0x7E00       ; Target Offset (Load to 0x7E00)
    dw 0x0000       ; Target Segment (0x0000)
    dq 1            ; LBA address (Start at sector 1, right after the boot sector) 

disk_error:
    mov ah, 0x0e
    mov al, 'E'
    int 0x10
    jmp $

msg: db "Booting PleiadOS", 0

times 510-($-$$) db 0 ; fill gap between end of program to 0xaa55 with 0
dw 0xaa55
