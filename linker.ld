SECTIONS
{
    . = 0x7c00;
    __start_of_mem = .;
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
    .end_of_elf :
    {
        *(.end_of_elf*);
    }
    . = ALIGN(4096);
    __end_of_mem = .;
}
