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

    ; variables

    ;global g_some_asm_var
    ;extern g_some_c_var

;g_some_asm_var dd ?

;***************************************************************************

    section .text

;RDI, RSI, RDX, RCX, R8 a R9

   global triple_nums
triple_nums:
    mov R8, dword 0
    mov R9D, dword 3
    mov R10, RDX

.back:
    cmp R8, R10
    je .end

    mov EAX, [ RDI + R8 * 4 ]
    imul R9D
    SHL RDX, 32
    OR RAX, RDX
    mov [RSI + R8 * 8], RAX

    inc R8
    jmp .back
.end:
    ret

;RDI, RSI, RDX, RCX, R8 a R9
    global make_divisible
make_divisible:
    mov R8, dword 0
    mov R11D, EDX
.back:
    cmp R8, RSI
    je .end

    mov EDX, dword 0
    mov EAX, [ RDI + R8 * 4 ]
    div R11D

    cmp EDX, 0
    je .skip

    mov R10D, R11D
    sub R10D, EDX
    add R10D, [ RDI + R8 * 4 ]
    mov [ RDI + R8 * 4 ], R10D

.skip:
    inc R8
    jmp .back
.end:
    ret

;RDI, RSI, RDX, RCX, R8 a R9
    global fibonacci
fibonacci:
    mov R8D, dword 0

    mov ESI, dword 0
    mov R9D, dword 1
    mov R10D, dword 0
.back:
    mov EAX, ESI

    cmp R8, RDI
    je .end

    mov R10D, R9D
    add R10D, ESI
    mov ESI, R9D
    mov R9D, R10D

    inc R8
    jmp .back
.end:
    ret

;RDI, RSI, RDX, RCX, R8 a R9
    global is_bnum_valid
is_bnum_valid:
    mov R10D, dword 1
    mov RCX, qword 1_000_000_000
    mov R9D, dword 0
    mov R8, dword 11

    mov RAX, RDI
    mov RDX, dword 0
    div RCX
    cmp RCX, qword 0
    cmove R10D, ECX

    mov RAX, RDI
    mov RDX, dword 0

    div R8
    cmp RDX, qword 0
    cmovne R10D, R9D

    mov EAX, R10D
.end:
    ret
