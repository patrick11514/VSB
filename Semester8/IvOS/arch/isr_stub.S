.text
.global isr_timer_stub

isr_timer_stub:
    pusha
    push $0
    call isr_handler_c
    add $4, %esp
    popa
    iret
