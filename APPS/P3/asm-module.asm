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

    ;rdi, rsi, rdx, rcx, r8, r9

    global suma_int
suma_int:
    mov eax, edi
    add eax, esi
    ret

    global suma_pole
suma_pole:
    movsx rsi, esi
    mov eax, 0
    mov rcx, 0
.back:
    cmp rcx, rsi
    je .end

    add eax, [rdi + rcx * 4]

    inc rcx
    jmp .back
.end:

    ret

    global pocet_cislic
pocet_cislic:
    mov eax, 0

.back:
    cmp [ rdi ], byte 0
    je .end
    
    cmp [ rdi ], byte '0'
    jl .skip
    cmp [ rdi ], byte '9'
    jg .skip

    inc eax

.skip:
    inc rdi
    jmp .back

.end:
    ret

    global max_pole
max_pole:
    movsx rsi, esi
    mov rax, [ rdi ]
    mov rcx, 1
.back:
    cmp rcx, rsi
    je .end

    cmp rax, [rdi + rcx * 8]
    cmovl rax, [rdi + rcx * 8]
    
    inc rcx
    jmp .back
.end:
    ret