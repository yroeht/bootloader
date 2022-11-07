SECTIONS
{
    . = 0x7c00;
    .bits16 :
    {
        *(.bits16*);
    }
    .text :
    {
        *(.text*);
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
