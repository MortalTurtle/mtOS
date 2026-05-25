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
    movl %ebp, (%eax)     # old->ebp = ebp
    movl %ebx, 4(%eax)    # old->ebx = ebx
    movl %esi, 8(%eax)    # old->esi = esi
    movl %edi, 12(%eax)   # old->edi = edi
    # Save return address as eip
    movl 0(%esp), %ecx    # ecx = return address (pushed by call instruction)
    movl %ecx, 16(%eax)   # old->eip = return address
    # Save current stack pointer
    movl %esp, 20(%eax)   # old->esp = esp
    # Load new context
    movl (%edx), %ebp     # new->ebp
    movl 4(%edx), %ebx    # new->ebx
    movl 8(%edx), %esi    # new->esi
    movl 12(%edx), %edi   # new->edi
    movl 16(%edx), %ecx   # new->eip
    movl 20(%edx), %esp   # new->esp
    # Jump to new eip
    jmp *%ecx
