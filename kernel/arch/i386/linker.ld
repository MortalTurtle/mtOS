ENTRY(_start)

SECTIONS
{
    . = 2M;

    kernel_physical_start = .;

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        __stack_chk_guard = .;
        LONG(0xDEADBEEF);  /* default value, changed in _start */
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }

    kernel_physical_end = .;
}
