#include "paging.h"
#include "lib/print.h"

#define SIZEOF_ARRAY(arr) (sizeof arr / sizeof *arr)

static union page_directory_entry pd[1024] __attribute__((aligned(4096)));
static union page_table_entry pt[1024] __attribute__((aligned(4096)));

static void paging_enable(void)
{
	asm volatile("mov %0, %%cr3" : : "r"(pd));

	asm volatile("mov %%cr0, %%eax" : : : "eax");
	//asm volatile("or $0x80000000, %%eax" : : : "eax");
	asm volatile("mov %%eax, %%cr0" :);
}

void paging_setup(void)
{
	int i;
	for(i = 0; i < 1024; i++)
	{
		pd[i].bytes = 0;
		pd[i].write = 1;
	}

	for(i = 0; i < 1024; i++)
	{
		pd[i].bytes = 0;
		pt[i].address_bits_31_12 = i;
		pt[i].supervisor = 1;
		pt[i].write = 1;
	}

	pd[0].bytes = ((unsigned)pt) | 3;

	paging_enable();
}

void paging_dump(const char *unused)
{
	(void) unused;

	long cr3;
	asm volatile("mov %%cr3, %%eax" : : : "eax");
	asm volatile("mov %%eax, %0" : "=m"(cr3));

	printk("cr3 %p\r\n", cr3);

	for (unsigned pdi = 0; pdi < SIZEOF_ARRAY(pd); ++pdi)
	{
		if (!pd[pdi].present)
			continue;
		printk("dir %p ", &pd[pdi]);
		if (pd[pdi].write)
			printk("write ");
		if (pd[pdi].supervisor)
			printk("supervisor ");
		if (pd[pdi].write_through)
			printk("write-through ");
		if (pd[pdi].cache_disable)
			printk("cache-disable ");
		if (pd[pdi].accessed)
			printk("accessed ");
		if (pd[pdi].available0)
			printk("available0 ");
		if (pd[pdi].page_size_4M)
			printk("page-size-4M ");
		if (pd[pdi].available)
			printk("available ");
		if (pd[pdi].page_attribute_table)
			printk("page-attribute-table" );
		printk("\r\n");

		union page_table_entry *pt = (union page_table_entry *)
			(pd[pdi].address_bits_31_12 << 12);

		printk("  table %p\r\n", pt);

		long attributes_save = pt[0].bytes & 0xfff;
		long first_address = pt[0].address_bits_31_12 << 12;
		for (unsigned pti = 1; pti < 1024; ++pti)
		{
			long attributes = pt[pti].bytes & 0xfff;
			long cur_address = pt[pti].address_bits_31_12 << 12;

			if (pti < 1023 && attributes == attributes_save)
				continue;

			printk("    %p -> %p ", first_address, cur_address);
			if (pt[pti].present)
				printk("present ");
			if (pt[pti].write)
				printk("write ");
			if (pt[pti].supervisor)
				printk("supervisor ");
			if (pt[pti].write_through)
				printk("write-through ");
			if (pt[pti].cache_disable)
				printk("cache-disable ");
			if (pt[pti].accessed)
				printk("accessed ");
			if (pt[pti].dirty)
				printk("dirty ");
			if (pt[pti].page_attribute)
				printk("page-attribute" );
			if (pt[pti].global)
				printk("global ");
			if (pt[pti].available)
				printk("available ");

			printk("\r\n");

			attributes_save = attributes;
			first_address = cur_address;
		}
	}
}
