SECTIONS
{
    . = 0x7c00;
    .text :
    {
        mem_start = .;
        *(.boot*);
        *(.text*);
        *(.bits16*);
    }
    .data :
    {
        *(.data*);
    }
    .rodata :
    {
        *(.rodata*);
    }
    .bss :
    {
        *(.bss*);
    }
}
