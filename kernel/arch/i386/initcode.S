.global start
start:
    pushl $argv
    pushl $init_path
    pushl $0            # dummy return address
    movl $1, %eax       # SYS_exec
    int $0x80           # syscall

exit_loop:
    movl $2, %eax       # SYS_exit
    int $0x80
    jmp exit_loop

init_path:
    .string "/init\0"

.p2align 2
argv:
    .long init_path
    .long 0

