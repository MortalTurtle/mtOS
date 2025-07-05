.macro isr_stub_no_error index
    .global isr_stub_\index
    isr_stub_\index:
        pushl $0                     # fictional error code
        pushl $\index                # exception number
        jmp isr_common_handler
.endm

.macro isr_stub_err index
    .global isr_stub_\index
    isr_stub_\index:
        pushl $\index                # excption num (error code already on stack)
        jmp isr_common_handler
.endm

isr_stub_no_error 0
isr_stub_no_error 1
isr_stub_no_error 2
isr_stub_no_error 3
isr_stub_no_error 4
isr_stub_no_error 5
isr_stub_no_error 6
isr_stub_no_error 7
isr_stub_err 8
isr_stub_no_error 9
isr_stub_err 10
isr_stub_err 11
isr_stub_err 12
isr_stub_err 13
isr_stub_err 14
isr_stub_no_error 15
isr_stub_no_error 16
isr_stub_err 17
isr_stub_no_error 18
isr_stub_no_error 19
isr_stub_no_error 20
isr_stub_no_error 21
isr_stub_no_error 22
isr_stub_no_error 23
isr_stub_no_error 24
isr_stub_no_error 25
isr_stub_no_error 26
isr_stub_no_error 27
isr_stub_no_error 28
isr_stub_no_error 29
isr_stub_err 30
isr_stub_no_error 31
isr_stub_no_error 32
isr_stub_no_error 33
isr_stub_no_error 34
isr_stub_no_error 35
isr_stub_no_error 36
isr_stub_no_error 37
isr_stub_no_error 38
isr_stub_no_error 39
isr_stub_no_error 40
isr_stub_no_error 41
isr_stub_no_error 42
isr_stub_no_error 43
isr_stub_no_error 44
isr_stub_no_error 45
isr_stub_no_error 46
isr_stub_no_error 47

isr_common_handler:
    pusha

    xor %eax, %eax
    mov %ds, %ax
    push %eax

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    
    push %esp
    call exception_handler
    add $4, %esp            # pop num of interrupt and error code from stack

    pop %eax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    popa
    add $8, %esp
    iret

.global isr_table
isr_table:
    .long isr_stub_0, isr_stub_1, isr_stub_2, isr_stub_3
    .long isr_stub_4, isr_stub_5, isr_stub_6, isr_stub_7
    .long isr_stub_8, isr_stub_9, isr_stub_10, isr_stub_11
    .long isr_stub_12, isr_stub_13, isr_stub_14, isr_stub_15
    .long isr_stub_16, isr_stub_17, isr_stub_18, isr_stub_19
    .long isr_stub_20, isr_stub_21, isr_stub_22, isr_stub_23
    .long isr_stub_24, isr_stub_25, isr_stub_26, isr_stub_27
    .long isr_stub_28, isr_stub_29, isr_stub_30, isr_stub_31
    .long isr_stub_32, isr_stub_33, isr_stub_34, isr_stub_35
    .long isr_stub_36, isr_stub_37, isr_stub_38, isr_stub_39
    .long isr_stub_40, isr_stub_41, isr_stub_42, isr_stub_43
    .long isr_stub_44, isr_stub_45, isr_stub_46, isr_stub_47