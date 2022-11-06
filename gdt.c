#include "gdt.h"

extern long start32;

union gdt_entry gdt[segment_gdt_idx_max];
struct gdt_descriptor gdt_descriptor;
const short kernel_cs = kernel_code * sizeof (union gdt_entry);
const short kernel_ds = kernel_data * sizeof (union gdt_entry);

void init_gdt(void)
{
	gdt[null].w1 = 0;
	gdt[null].w2 = 0;

	for (int i = 0; i < segment_gdt_idx_max; ++i)
	{
		gdt[i].base_low = 0;
		gdt[i].base_mid = 0;
		gdt[i].base_hi = 0;
		gdt[i].limit_low = ~0;
		gdt[i].limit_hi = ~0;
		gdt[i].granularity = one_page;
		gdt[i].size = bits_32;
		gdt[i].long_mode = 0;
		gdt[i].reserved = 0;
		gdt[i].present = 1;
		gdt[i].type = code_or_data;
		gdt[i].accessed = 0;
	}

	gdt[kernel_code].dpl = 0;
	gdt[kernel_code].executable = 1;
	gdt[kernel_code].rw = 1;
	gdt[kernel_code].conforming_or_direction = 0;

	gdt[kernel_data].dpl = 0;
	gdt[kernel_data].executable = 0;;
	gdt[kernel_data].rw = 1;
	gdt[kernel_data].conforming_or_direction = expand_up;

	gdt[user_code].dpl = 3;
	gdt[user_code].executable = 1;
	gdt[user_code].rw = 1;
	gdt[user_code].conforming_or_direction = 0;

	gdt[user_data].dpl = 3;
	gdt[user_data].executable = 0;
	gdt[user_data].rw = 1;
	gdt[user_data].conforming_or_direction = expand_up;

	gdt_descriptor.size = sizeof gdt - 1;
	gdt_descriptor.address = (long)gdt;

	asm volatile("cli");
	asm volatile("lgdt (%0)" : : "m"(gdt_descriptor));
	asm volatile("mov %%cr0, %%eax" :);
	asm volatile("or $1, %%al" :);
	asm volatile("mov %%eax, %%cr0" :);
	asm volatile("ljmp $0x08, $%0" : : "m"(start32));
}
