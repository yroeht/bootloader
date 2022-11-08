#include "idt.h"
#include "pic.h"
#include "print.h"

extern short number_extra_sectors;

#define MAGIC_CHECK 0x12345678

long magic_check __attribute__((section(".end_of_elf"))) = MAGIC_CHECK;

void kernel_entry(void)
{
	reset_colorcode();
	print_string("Welcome to 32 bits Protected Mode!\r\n");
	printk("Loaded %p extra sectors after bootsector.\r\n",
			number_extra_sectors);
	if (magic_check != MAGIC_CHECK)
	{
		printk("Check of magic number failed, "
				"address %p read %p (expected %p) "
				"end of ELF is not loaded properly.\r\n",
				&magic_check, magic_check, MAGIC_CHECK);
		for (;;)
			continue;
	}
	printk("Magic check validated, ELF loaded until %p\r\n", &magic_check);
	pic_init();
	init_idt();
	asm volatile ("int $0x1");
	asm volatile ("int $0x81");
	for (;;)
		continue;
}
