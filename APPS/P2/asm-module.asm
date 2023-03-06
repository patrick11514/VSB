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

    extern g_int_pole, g_suma

    extern g_string, g_strlen

    extern g_max

;g_some_asm_var dd ?

;***************************************************************************

    section .text

    global suma_pole
suma_pole:
    ; for (int i = 0; i < 10; i++)
    mov rdx, 0 ; i = 0
    mov [g_suma], dword 0
    mov eax, 0 ; tmp_sum
.back:
    cmp rdx, 10 ; i < 10
    jg .done

    add eax, [g_int_pole + rdx * 4] ; tmp_sum += g_int_pole[i]

    inc rdx ;i++
    jmp .back

.done:
    mov [g_suma], eax ; g_suma = tmp_sum
    ret

    global get_len
get_len:

    mov rdx, 0
    mov [g_strlen], dword 0
    mov eax, 0

.back:
    cmp [ g_string + rdx ], byte 0
    je .done
    inc rdx
    inc eax
    jmp .back
.done:
    mov [g_strlen], eax
    ret

    global search_max
search_max:
    mov rdx, dword 0 ; i = 0
    mov eax, [g_int_pole] ; tmp_max = g_int_pole [0]
.back:
    cmp rdx, 10
    jg .done

    cmp eax, [ g_int_pole + rdx * 4 ]
    jg .skip ; if (tmp_max > g_int_pole[i]) skip
    mov eax, [ g_int_pole + rdx * 4 ] ; tmp_max = g_int_pole[i]

.skip:
    inc rdx
    jmp .back
.done:
    mov [ g_max] , eax
    ret