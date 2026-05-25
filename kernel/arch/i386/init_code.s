.section .text
.globl _start
_start:
    pushl $argv
    pushl $init
    pushl $0
    movl $1, %eax    # SYS_exec system call number
    int $0x80        # System call interrupt
    # If exec fails, exit
    movl $2, %eax    # SYS_exit
    int $0x80

.data
init:
    .string "/init\0"
argv:
    .long init
    .long 0
