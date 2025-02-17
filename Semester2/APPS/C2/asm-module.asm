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

    extern g_string, g_longs, g_ints, g_count

;***************************************************************************

    section .text

    global fixtext
fixtext:
    mov RBX, 0
.back:
    mov AL, [g_string + RBX]
    cmp AL, 0
    je .end
    cmp AL, '_'
    jne .skip
    mov [g_string + RBX], byte 'e'
.skip:
    INC RBX
    jmp .back
.end:
    ret

    global doubleit
doubleit:
    mov RBX, 0
.back:
    cmp RBX, 6
    je .end

    mov RAX, [g_longs + RBX * 8] 
    SHL RAX, 1
    mov [g_longs + RBX * 8], RAX
    INC RBX
    jmp .back
.end:
    ret

    global count
count:
    mov RBX, 0
    mov ECX, 0
.back:
    cmp RBX, 9
    je .end

    mov EAX, [g_ints + RBX * 4]
    and EAX, 0b0011

    cmp EAX, dword 0
    jne .skip
    INC ECX
.skip:
    INC RBX
    jmp .back
.end:
    mov [g_count], ECX
    ret