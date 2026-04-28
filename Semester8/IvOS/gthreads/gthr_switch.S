#
#   Context switch for x86_64
#
#   %rdi contains the current context structure.
#   %rsi contains the context to switch to.
#
#   rbp is the base pointer, which points to the base of the current stack frame
#   rsp is the stack pointer, which points to the top of the current stack frame
#   The called routine is expected to preserve rsp, rbp, rbx, r12, r13, r14, and r15 but may trample any other registers.
#
#   see https://software.intel.com/en-us/articles/introduction-to-x64-assembly


.globl _gt_switch, gt_switch
_gt_switch:
gt_switch:

        mov     %rsp, 0x00(%rdi)
        mov     %r15, 0x08(%rdi)
        mov     %r14, 0x10(%rdi)
        mov     %r13, 0x18(%rdi)
        mov     %r12, 0x20(%rdi)
        mov     %rbx, 0x28(%rdi)
        mov     %rbp, 0x30(%rdi)

        mov     0x00(%rsi), %rsp
        mov     0x08(%rsi), %r15
        mov     0x10(%rsi), %r14
        mov     0x18(%rsi), %r13
        mov     0x20(%rsi), %r12
        mov     0x28(%rsi), %rbx
        mov     0x30(%rsi), %rbp

        ret
