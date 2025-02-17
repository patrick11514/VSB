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

;***************************************************************************

    section .text

;RDI, RSI, RDX, RCX, R8 a R9

    global sum_arrays
sum_arrays:
    mov RAX, dword 0
    movsx RCX, ECX

.back:
    cmp RAX, RCX
    je .end

    mov R8D, [RDI + RAX * 4]
    add R8D, dword [RSI + RAX * 4]
    mov [RDX + RAX * 4], R8D

    inc RAX
    jmp .back
.end:
    ret

;RDI, RSI, RDX, RCX, R8 a R9

    global concat_strings
concat_strings:
    mov EAX, dword 0
    mov R8, dword 0
    mov R9, RDI
.back:
    cmp [R9 + R8], byte 0
    je .switch

    mov CL, [R9 + R8]
    mov [RDX + RAX], CL

    inc EAX
    inc R8

    jmp .back

.switch:
    cmp RSI, R9
    je .end

    mov R9, RSI
    mov R8, dword 0
    jmp .back

.end:
    ret

;RDI, RSI, RDX, RCX, R8 a R9
    global num_to_bin_str
num_to_bin_str:
    mov [RSI], byte '0'
    mov [RSI + 1], byte 'b'
    mov RAX, dword 2
    mov ECX, 1 << 31
    mov R8D, dword 0
    mov R9D, dword 0

.back:
    cmp R8D, 32
    je .end
    
    test EDI, ECX
    je .zero
    jmp .one
.one:
    mov [RSI + RAX], byte '1'
    mov R9D, dword 1
    inc RAX
    jmp .shift
.zero:
    cmp R9D, dword 0
    je .shift
    mov [RSI + RAX], byte '0'
    inc RAX
.shift:
    shr ECX, 1
    inc R8D
    jmp .back

.end:
    ret
