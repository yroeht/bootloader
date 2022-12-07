#include "ata.h"
#include "idt.h"
#include "pic.h"
#include "lib/print.h"
#include "fat.h"
#include "string.h"
#include "putc.h"
#include "shell.h"
#include "paging.h"

short number_extra_sectors __attribute__((section(".bits16.data"))) = NUM_SECTORS;

#define MAGIC_CHECK 0x12345678

long magic_check __attribute__((section(".end_of_elf"))) = MAGIC_CHECK;

short sector[256];

void kernel_entry(void)
{
	reset_colorcode();
	pic_init();
	init_idt();
	paging_setup();
	if (magic_check != MAGIC_CHECK)
	{
		printk("Check of magic number failed, "
				"address %p read %p (expected %p) "
				"end of ELF is not loaded properly.\r\n",
				&magic_check, magic_check, MAGIC_CHECK);
		for (;;)
			continue;
	}

	fs_init();

	screen_reset();
	shell_reset();

	for (;;)
		shell_process();
}
