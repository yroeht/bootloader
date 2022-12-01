#include "paging.h"
#include "lib/print.h"

#define SIZEOF_ARRAY(arr) (sizeof arr / sizeof *arr)

#define PAGE_SIZE 4096

static union page_directory_entry pd[1024] __attribute__((aligned(PAGE_SIZE)));

extern int __end_of_mem;
extern int __start_of_mem;

#define PHYS_MEM_AVAILABLE 0x10000000 // 8M
#define PAGE_FRAMES (PHYS_MEM_AVAILABLE / PAGE_SIZE)

static char pf[PAGE_FRAMES];

static int pf_alloc(void)
{
	for (int i = 0; i < PAGE_FRAMES; ++i)
	{
		if (!pf[i])
		{
			pf[i] = 1;
			char *pfp = (char *)(i << 12);
			for (int j = 0; j < PAGE_SIZE; ++j)
				pfp[j] = 0;
			map((int)pfp, (int)pfp, PAGE_SIZE);
			return i;
		}
	}
	return -1;
}

static union page_table_entry *pt_alloc(void)
{
	int i = pf_alloc();

	if (i < 0)
		return 0;
	return (union page_table_entry *)(i * PAGE_SIZE);
}

static void paging_enable(void)
{
	asm volatile("mov %0, %%cr3" : : "r"(pd));

	asm volatile("mov %%cr0, %%eax" : : : "eax");
	asm volatile("or $0x80000000, %%eax" : : : "eax");
	asm volatile("mov %%eax, %%cr0" :);
}

void map(int virt, int phys, int size)
{
	int pdi = virt >> 22;
	int pti = (virt >> 12) & 0x3ff;
	union page_table_entry *ptp;

	if (pd[pdi].present)
		ptp = (union page_table_entry *)(pd[pdi].address_bits_31_12 << 12);
	else
	{
		ptp = pt_alloc();
		pd[pdi].present = 1;
		pd[pdi].address_bits_31_12 = (int)ptp >> 12;
	}

	for (int i = 0; i < 1024 && i * PAGE_SIZE < size; ++i)
	{
		ptp[pti + i].present = 1;
		ptp[pti + i].write = 1;
		ptp[pti + i].supervisor = 1;
		ptp[pti + i].address_bits_31_12 = (phys >> 12) + i;
	}
}

void paging_setup(void)
{
	int mapped_start_page = ((int)&__start_of_mem >> 12);
	int mapped_end_page = ((int)&__end_of_mem >> 12) + 1;

	for (int i = 0; i < PAGE_FRAMES; ++i)
		pf[i] = 0;
	for (int i = mapped_start_page; i < mapped_end_page; ++i)
		pf[i] = 1;

	for (int i = 0; i < 1024; i++)
		pd[i].bytes = 0;

	static union page_table_entry pt[1024] __attribute__((aligned(PAGE_SIZE)));

	pd[0].present = 1;
	pd[0].address_bits_31_12 = (int)pt >> 12;

	int mapped_start_address = mapped_start_page * PAGE_SIZE;
	int mapped_size = (mapped_end_page - mapped_start_page) * PAGE_SIZE;

	// The OS's intially loaded image
	map(mapped_start_address, mapped_start_address, mapped_size);
	// The VGA framebuffer
	map(0xb8000, 0xb8000, 80 * 24 * 2);
	// The stack
	map(0xf00000, 0xf00000, 0x100000);

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
		printk("dir %p pdi %d ", &pd[pdi], pdi);
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

	int pf_allocated = 0;
	for (int i = 0; i < PAGE_FRAMES; ++i)
		if (pf[i])
			++pf_allocated;
	printk("page_frames allocated : %d/%d\r\n", pf_allocated, PAGE_FRAMES);
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


