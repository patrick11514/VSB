;***************************************************************************
;
; Program for education in subject "Assembly Languages" and "APPS"
; petr.olivka@vsb.cz, Department of Computer Science, VSB-TUO
;
; Empty project
;
;***************************************************************************

    bits 64

    section .data

    extern g_string

    extern g_number

    extern g_int
    extern g_long

;g_some_asm_var dd ?

;***************************************************************************

    section .text

    global fix_text

fix_text:
    mov byte [ g_string + 1 ], 'e'
    mov byte [ g_string + 9 ], 'e'
    mov byte [ g_string + 16 ], 'e'
    ret

    global change_endianity

change_endianity:
    mov AL, [ g_number + 0 ]
    mov BL, [ g_number + 3 ]
    mov [ g_number + 3 ], AL
    mov [ g_number + 0 ], BL

    mov AL, [ g_number + 1 ]
    mov BL, [ g_number + 2 ]
    mov [ g_number + 2 ], AL
    mov [ g_number + 1 ], BL
    ret

    global transfer_number

transfer_number:
    movsx RAX, dword [ g_int ]
    mov [ g_long ], RAX
    ret
