# kernel/arch/i386/swtch.s
.section .text
.globl swtch
swtch:
    # Save old context
    # void swtch(struct context** old, struct context* new);
    # 4(%esp) = &old
    # 8(%esp) = new
    movl 4(%esp), %eax    # eax = &old
    movl 8(%esp), %edx    # edx = new
    # Save old registers

    pushl %ebp
    pushl %ebx
    pushl %esi
    pushl %edi

    movl %esp, (%eax)
    movl %edx, %esp

    popl %edi
    popl %esi
    popl %ebx
    popl %ebp

    ret

