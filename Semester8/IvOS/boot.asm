[bits 16]    ; 16 bit mode
[org 0x7c00] ; starting addr

boot:
    hlt

times 510-($-$$) db 0 ; fill gap between end of program to 0xaa55 with 0
dw 0xaa55