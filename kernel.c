#include "idt.h"
#include "pic.h"
#include "print.h"

extern short number_extra_sectors;

void kernel_entry(void)
{
	reset_colorcode();
	print_string("Welcome to 32 bits Protected Mode!\r\n");
	printk("Loaded %p extra sectors after bootsector.\r\n",
			number_extra_sectors);
	pic_init();
	init_idt();
	asm volatile ("int $0x1");
	asm volatile ("int $0x81");
	for (;;)
		continue;
}
