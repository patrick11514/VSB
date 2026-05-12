.text
    .global kt_switch
    .extern kt_do_memory_swap

/* kt_switch(old_esp_ptr, new_esp_val) */
kt_switch:
    pushf
    pusha                # Save all 8 general purpose registers (32 bytes)

    mov 40(%esp), %eax   # Load old_esp_ptr
    mov 44(%esp), %edx   # Load new_esp_val

    mov %esp, (%eax)     # Save the current ESP into *old_esp_ptr

    # --- THE TRAMPOLINE ---
    # The stack is now 100% saved. We safely trigger the memory swap!
    # We must push %edx so the C function doesn't accidentally overwrite it.
    push %edx            
    call kt_do_memory_swap
    pop %edx             
    # ----------------------

    mov %edx, %esp       # SWAP TO NEW STACK!

    popa                 # Restore all 8 registers for the NEW thread!
    popf                 # Restore EFLAGS
    ret