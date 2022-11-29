#include "ata.h"
#include "idt.h"
#include "pic.h"
#include "lib/print.h"
#include "fat.h"
#include "string.h"
#include "putc.h"
#include "shell.h"

short number_extra_sectors __attribute__((section(".bits16.data"))) = NUM_SECTORS;

extern long __start_of_mem;

#define MAGIC_CHECK 0x12345678

long magic_check __attribute__((section(".end_of_elf"))) = MAGIC_CHECK;

short sector[256];

void kernel_entry(void)
{
	reset_colorcode();
	paging_setup();
	pic_init();
	init_idt();
	printk("Welcome to 32 bits Protected Mode!\r\n");
	printk("Loaded %d extra sectors after bootsector.\r\n",
			number_extra_sectors);
	printk("%.*s %d %*.x %d\r\n", 4, "Teststring", 42, 8, 0x1234, 0);
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
	asm volatile ("int $0x1");
	asm volatile ("int $0x81");

	ata_lba_read(0, 1, sector);
	printk("Magic according to ATA read of sector 0: %p\r\n", sector[255] & 0xffff);

	fs_init();

	shell_reset();

	for (;;)
		continue;
}
