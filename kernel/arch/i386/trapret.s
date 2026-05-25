.global trapret
trapret:
    # Restore data segments
    popl %gs
    popl %fs
    popl %es
    popl %ds
    # Restore general purpose registers
    popal
    # Skip interrupt and error code
    addl $8, %esp
    # Return to user mode
    iret
