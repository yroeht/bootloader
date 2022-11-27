#include "paging.h"

static union page_directory_entry pd[1024] __attribute__((aligned(4096)));
static union page_table_entry pt[1024] __attribute__((aligned(4096)));

static void paging_enable(void)
{
	asm volatile("mov %0, %%cr3" : : "r"(pd));

	asm volatile("mov %%cr0, %%eax" : : : "eax");
	asm volatile("or $0x80000000, %%eax" : : : "eax");
	asm volatile("mov %%eax, %%cr0" :);
}

void paging_setup(void)
{
	//set each entry to not present
	int i;
	for(i = 0; i < 1024; i++)
	{
		// This sets the following flags to the pages:
		//   Supervisor: Only kernel-mode can access them
		//   Write Enabled: It can be both read from and written to
		//   Not Present: The page table is not present
		pd[i].bytes = 0x00000002;
	}

	// i holds the physical address where we want to start mapping these pages to.
	// in this case, we want to map these pages to the very beginning of memory.
	//we will fill all 1024 entries in the table, mapping 4 megabytes
	for(i = 0; i < 1024; i++)
	{
		// As the address is page aligned, it will always leave 12 bits zeroed.
		// Those bits are used by the attributes ;)
		pt[i].bytes = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
	}

	pd[0].bytes = ((unsigned)pt) | 3;

	paging_enable();
}
