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
    mov ax, 0x0000 ; AX used as temporary storage for es
    mov es, ax     ; es cannot be directly assigned

    mov bx, 0x7e00 ; load to memory after 512 bytes which bios loaded
    mov al, 80     ; load 80 sectors

    mov ch, 0 ; CHS cylinder 0
    mov cl, 2 ; CHS CC SSSSSS -> sector 2
    mov dl, 0x80 ; CHS drive number
    mov dh, 0 ; CHS head 0

    ; let's read sector by sector instead of 40 at once to be safe, or just read 40.
    ; wait, maybe just 40 works, but if it fails, let's catch it.
    mov ah, 0x02 ; read desired sectors into memory

    int 0x13
    jc disk_error

    jmp 0x0000:0x7e00

disk_error:
    mov ah, 0x0e
    mov al, 'E'
    int 0x10
    jmp $

msg: db "Booting PleiadOS", 0

times 510-($-$$) db 0 ; fill gap between end of program to 0xaa55 with 0
dw 0xaa55