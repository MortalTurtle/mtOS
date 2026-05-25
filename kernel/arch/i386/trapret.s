.section .text
.global trapret
trapret:
    # Restore data segments
    popl %ds
    popl %es
    popl %fs
    popl %gs
    # Restore general purpose registers
    popal
    # Skip interrupt and error code
    addl $8, %esp
    # Return to user mode
    iret
