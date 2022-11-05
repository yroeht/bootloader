#include "print.h"
#include "idt.h"

extern char number_extra_sectors;

void kernel_entry(void)
{
	reset_colorcode();
	print_string("Hello from kernel_entry!\r\n");
	print_string("Loaded ");
	print_hex(number_extra_sectors);
	print_string(" extra sectors after bootsector.\r\n");
	printk("%s: T%sing %s %a(%%s and %%p): %p\r\n",
			__func__, "est", "printf", 42);
	init_idt();
	asm volatile ("int $0x80");
	for (;;)
		continue;
}
