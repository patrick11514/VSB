[bits 16]    ; 16 bit mode
[org 0x7e00] ; starting addr

kernel_message:
    mov si, msg
    mov bp, msg

    mov ah, 0x03
    int 0x10

    mov bx, 0x02
    mov ah, 0x13 
    mov al, 0x01
    mov cx, 17
    int 0x10

    mov ax, 0
    cpuid

halt:    
    hlt

msg: db "Loading kernel...", 0