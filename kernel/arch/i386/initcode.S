.global start
start:
    movl $0xDEADBEEF, %eax
    movl $0xDEADBEEF, %ebx
1:
    jmp 1b
