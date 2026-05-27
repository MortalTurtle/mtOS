.global trapret
trapret:
    popal
    # Restore data segments
    popl %gs
    popl %fs
    popl %es
    popl %ds
    # Restore general purpose registers
    # Skip interrupt and error code
    addl $8, %esp
    # Return to user mode
    iret
