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
isr_stub_no_error 48
isr_stub_no_error 49
isr_stub_no_error 50
isr_stub_no_error 51
isr_stub_no_error 52
isr_stub_no_error 53
isr_stub_no_error 54
isr_stub_no_error 55
isr_stub_no_error 56
isr_stub_no_error 57
isr_stub_no_error 58
isr_stub_no_error 59
isr_stub_no_error 60
isr_stub_no_error 61
isr_stub_no_error 62
isr_stub_no_error 63
isr_stub_no_error 64
isr_stub_no_error 65
isr_stub_no_error 66
isr_stub_no_error 67
isr_stub_no_error 68
isr_stub_no_error 69
isr_stub_no_error 70
isr_stub_no_error 71
isr_stub_no_error 72
isr_stub_no_error 73
isr_stub_no_error 74
isr_stub_no_error 75
isr_stub_no_error 76
isr_stub_no_error 77
isr_stub_no_error 78
isr_stub_no_error 79
isr_stub_no_error 80
isr_stub_no_error 81
isr_stub_no_error 82
isr_stub_no_error 83
isr_stub_no_error 84
isr_stub_no_error 85
isr_stub_no_error 86
isr_stub_no_error 87
isr_stub_no_error 88
isr_stub_no_error 89
isr_stub_no_error 90
isr_stub_no_error 91
isr_stub_no_error 92
isr_stub_no_error 93
isr_stub_no_error 94
isr_stub_no_error 95
isr_stub_no_error 96
isr_stub_no_error 97
isr_stub_no_error 98
isr_stub_no_error 99
isr_stub_no_error 100
isr_stub_no_error 101
isr_stub_no_error 102
isr_stub_no_error 103
isr_stub_no_error 104
isr_stub_no_error 105
isr_stub_no_error 106
isr_stub_no_error 107
isr_stub_no_error 108
isr_stub_no_error 109
isr_stub_no_error 110
isr_stub_no_error 111
isr_stub_no_error 112
isr_stub_no_error 113
isr_stub_no_error 114
isr_stub_no_error 115
isr_stub_no_error 116
isr_stub_no_error 117
isr_stub_no_error 118
isr_stub_no_error 119
isr_stub_no_error 120
isr_stub_no_error 121
isr_stub_no_error 122
isr_stub_no_error 123
isr_stub_no_error 124
isr_stub_no_error 125
isr_stub_no_error 126
isr_stub_no_error 127
isr_stub_no_error 128

.global isr_common_handler

isr_common_handler:
    # segment registers
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    # general registers
    pusha

    # switch to kernel segments
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    # pass trapframe*
    pushl %esp
    call exception_handler
    addl $4, %esp

    # restore registers
    popa

    popl %gs
    popl %fs
    popl %es
    popl %ds

    # remove trapno + err
    addl $8, %esp

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
    .long isr_stub_48, isr_stub_49, isr_stub_50, isr_stub_51
    .long isr_stub_52, isr_stub_53, isr_stub_54, isr_stub_55
    .long isr_stub_56, isr_stub_57, isr_stub_58, isr_stub_59
    .long isr_stub_60, isr_stub_61, isr_stub_62, isr_stub_63
    .long isr_stub_64, isr_stub_65, isr_stub_66, isr_stub_67
    .long isr_stub_68, isr_stub_69, isr_stub_70, isr_stub_71
    .long isr_stub_72, isr_stub_73, isr_stub_74, isr_stub_75
    .long isr_stub_76, isr_stub_77, isr_stub_78, isr_stub_79
    .long isr_stub_80, isr_stub_81, isr_stub_82, isr_stub_83
    .long isr_stub_84, isr_stub_85, isr_stub_86, isr_stub_87
    .long isr_stub_88, isr_stub_89, isr_stub_90, isr_stub_91
    .long isr_stub_92, isr_stub_93, isr_stub_94, isr_stub_95
    .long isr_stub_96, isr_stub_97, isr_stub_98, isr_stub_99
    .long isr_stub_100, isr_stub_101, isr_stub_102, isr_stub_103
    .long isr_stub_104, isr_stub_105, isr_stub_106, isr_stub_107
    .long isr_stub_108, isr_stub_109, isr_stub_110, isr_stub_111
    .long isr_stub_112, isr_stub_113, isr_stub_114, isr_stub_115
    .long isr_stub_116, isr_stub_117, isr_stub_118, isr_stub_119
    .long isr_stub_120, isr_stub_121, isr_stub_122, isr_stub_123
    .long isr_stub_124, isr_stub_125, isr_stub_126, isr_stub_127
    .long isr_stub_128
