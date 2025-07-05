.set ALIGN,    1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,  1<<1             /* provide memory map */
.set FLAGS,    ALIGN | MEMINFO  /* this is the Multiboot 'flag' field */
.set MAGIC,    0x1BADB002       /* 'magic number' lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */

.section .multiboot
    .align 4
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

.section .bss
    .align 16
stack_bottom:
    .skip 16384 # 16 KiB
stack_top:

.extern __stack_chk_guard

.section .text
.global _start
.type _start, @function
_start:
    #save multiboot data
    mov %ebx, %esi   # ESI = multiboot_info pointer
    mov %eax, %edi   # EDI = magic number
    mov $stack_top, %esp
    # init canary for stack smashing protection
    call init_stack_check_guard

    call _init
    cli
    call load_gdt
    call IDT_configure
    call load_idt
    call PIC_configure
    sti
    call kernel_main
    call _fini
    cli
1:  hlt
    jmp 1b

init_stack_check_guard:
    rdtsc               # EDX:EAX = TSC
    xor %ecx, %eax
    xor %esp, %eax
    xor 4(%esi), %eax    
    mov $__stack_chk_guard, %ecx
    mov %eax, (%ecx)
    ret

.extern gdt_ptr
load_gdt:
    lgdt [gdt_ptr]
    # Refresh code segment with far jump
    ljmp $0x08, $.reload_cs
.reload_cs:
    # Refresh data segments
    mov $0x10, %ax # Kernel data segment selector
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ret

.extern idt_ptr
load_idt:
    lidt [idt_ptr]
    ret

.size _start, . - _start
