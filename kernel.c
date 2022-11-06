#include "print.h"
#include "idt.h"

extern short number_extra_sectors;

extern void init_pic(void);

void kernel_entry(void)
{
	reset_colorcode();
	print_string("Welcome to 32 bits Protected Mode!\r\n");
	printk("Loaded %p extra sectors after bootsector.\r\n",
			number_extra_sectors);
	init_pic();
	init_idt();
	asm volatile ("int $0x1");
	for (;;)
		continue;
}
