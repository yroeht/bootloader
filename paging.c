#include "paging.h"
#include "lib/print.h"

#define SIZEOF_ARRAY(arr) (sizeof arr / sizeof *arr)

static union page_directory_entry pd[1024] __attribute__((aligned(4096)));

#define PT_CNT 4

static union page_table_entry pt_slab[PT_CNT][1024] __attribute__((aligned(4096)));

static int pt_slab_idx = 0;

union page_table_entry *pt_slab_alloc(void)
{
	if (pt_slab_idx >= PT_CNT)
		return 0;
	return pt_slab[pt_slab_idx++];
}

static void paging_enable(void)
{
	asm volatile("mov %0, %%cr3" : : "r"(pd));

	asm volatile("mov %%cr0, %%eax" : : : "eax");
	//asm volatile("or $0x80000000, %%eax" : : : "eax");
	asm volatile("mov %%eax, %%cr0" :);
}

static void map(int virt, int phys, int size)
{
	int pdi = virt >> 22;
	int pti = (virt >> 12) & 0x3ff;
	union page_table_entry *ptp;

	if (pd[pdi].present)
		ptp = (union page_table_entry *)(pd[pdi].address_bits_31_12 << 12);
	else
	{
		ptp = pt_slab_alloc();
		pd[pdi].present = 1;
		pd[pdi].address_bits_31_12 = (int)ptp >> 12;
	}

	for (int i = 0; i < 1024 && i * 4096 < size; ++i)
	{
		ptp[pti + i].present = 1;
		ptp[pti + i].write = 1;
		ptp[pti + i].supervisor = 1;
		ptp[pti + i].address_bits_31_12 = (phys >> 12) + i;
	}
}

void paging_setup(void)
{
	for (int i = 0; i < 1024; i++)
		pd[i].bytes = 0;

	for (int slab = 0; slab < PT_CNT; ++ slab)
		for (int i = 0; i < 1024; i++)
			pt_slab[slab][i].bytes = 0;

	map(0x7c00, 0x7c00, NUM_SECTORS * 512);
	map(0xb8000, 0xb8000, 80 * 24 * 2);

	paging_enable();
}

void paging_dump(const char *unused)
{
	(void) unused;

	long cr3;
	asm volatile("mov %%cr3, %%eax" : : : "eax");
	asm volatile("mov %%eax, %0" : "=m"(cr3));

	printk("allocated %d page tables\r\n", pt_slab_idx);
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

		union page_table_entry *pt_save = pt;
		int range_start_virt = (pdi << 22);
		int range_start_phys = pt[0].address_bits_31_12 << 12;

		for (unsigned pti = 0; pti < 1024; ++pti)
		{
			long range_cur_virt = (pdi << 22) | (pti << 12);
			long range_cur_phys = pt[pti].address_bits_31_12 << 12;
			long attributes_same =
				(pt_save->bytes & 0xfff) == (pt[pti].bytes & 0xfff);

			if (pti < 1023 && attributes_same)
				continue;

			printk("    %p:%p", range_start_virt, range_cur_virt);
			if (pt_save->present)
			{
				printk(" -> %p ", range_start_phys);
				if (pt_save->write)
					printk("write ");
				if (pt_save->supervisor)
					printk("supervisor ");
				if (pt_save->write_through)
					printk("write-through ");
				if (pt_save->cache_disable)
					printk("cache-disable ");
				if (pt_save->accessed)
					printk("accessed ");
				if (pt_save->dirty)
					printk("dirty ");
				if (pt_save->page_attribute)
					printk("page-attribute" );
				if (pt_save->global)
					printk("global ");
				if (pt_save->available)
					printk("available ");
			}

			printk("\r\n");

			pt_save = &pt[pti];
			range_start_virt = range_cur_virt;
			range_start_phys = range_cur_phys;
		}
	}
}

static long atoi(const char *s, int b)
{
	long ret = 0;
	for (; *s; ++s)
	{
		if (*s == ' ')
			continue;
		if ('0' <= *s && *s <= '9' && b > *s - '0')
			ret = ret * b + *s - '0';
		else if ('a' <= *s && *s <= 'z' && b > *s - 'a' + 10)
			ret = ret * b + *s - 'a' + 10;
		else if ('A' <= *s && *s <= 'Z' && b > *s - 'A' + 36)
			ret = ret * b + *s - 'A' + 36;
		else return -1;
	}
	return ret;
}

void paging_virtstr_to_phys(const char *arg)
{
	int virt = atoi(arg, 16);
	int phys = paging_virt_to_phys(virt);

	printk("%p\r\n", phys);
}

int paging_virt_to_phys(int virt)
{
	long pdi = virt >> 22;
	long pti = (virt >> 12) & 0x3ff;
	long off = virt & 0xfff;

	union page_directory_entry *pdb;

	asm volatile("mov %%cr3, %0" : "=r"(pdb));

	union page_table_entry * ptb = (union page_table_entry *)
		(pdb[pdi].address_bits_31_12 << 12);
	long phys = (ptb[pti].address_bits_31_12 << 12) | off;

	return phys;
}
