#include "print.h"

extern char number_extra_sectors;

void kernel_entry(void)
{
	reset_colorcode();
	print_string("Hello from kernel_entry!\r\n");
	print_string("Loaded ");
	print_hex(number_extra_sectors);
	print_string(" extra sectors after bootsector.\r\n");
	for (;;)
		continue;
}
