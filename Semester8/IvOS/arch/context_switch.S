    .text
    .global kt_switch
/* kt_switch(old_esp_ptr, new_esp_val)
 * cdecl: [esp+4]=old_esp_ptr, [esp+8]=new_esp_val
 * Saves current esp into *old_esp_ptr, sets esp=new_esp_val and returns.
 * Caller must have arranged new stack so that a `ret` resumes execution.
 */
kt_switch:
    mov 4(%esp), %eax    # load old_esp_ptr
    mov 8(%esp), %edx    # load new_esp_val
    mov %esp, (%eax)     # save current esp into *old_esp_ptr
    mov %edx, %esp       # switch to new stack
    ret
