#include "gdt.h"
#include "idt.h"
#include "isr.h"

static struct gate_descriptor idt[256];
static struct idt_descriptor idt_descriptor;

static const short kernel_cs = kernel_code * sizeof (union gdt_entry);

void init_idt(void)
{
	for (unsigned i = 0; i < sizeof idt / sizeof idt[0]; ++i)
	{
		idt[i].offset_lo =
			(int)default_interrupt_handler & 0xffff;
		idt[i].segment_selector = kernel_cs;
		idt[i].reserved0 = 0;
		idt[i].gate_type = gate_type_32bit_int;
		idt[i].reserved1 = 0;
		idt[i].dpl = 0;
		idt[i].present = 1;
		idt[i].offset_hi =
			(int)default_interrupt_handler >> 16;
	}

#define ISR(n, str, has_error, is_excn)                        \
	idt[n].offset_lo =                                     \
	     (int)interrupt_handler##n & 0xffff;               \
	idt[n].segment_selector = kernel_cs;                   \
	idt[n].reserved0 = 0;                                  \
	idt[n].gate_type = is_excn                             \
			? gate_type_32bit_trap                 \
			: gate_type_32bit_int;                 \
	idt[n].reserved1 = 0;                                  \
	idt[n].dpl = 0;                                        \
	idt[n].present = 1;                                    \
	idt[n].offset_hi =                                     \
		(int)interrupt_handler##n >> 16;               \

#include "isr-list.inc"

#undef ISR

	idt[65].offset_lo =
		(int)keyboard_handler & 0xffff;
	idt[65].segment_selector = kernel_cs;
	idt[65].reserved0 = 0;
	idt[65].gate_type = gate_type_32bit_int;
	idt[65].reserved1 = 0;
	idt[65].dpl = 0;
	idt[65].present = 1;
	idt[65].offset_hi =
		(int)keyboard_handler >> 16;

	idt[128].offset_lo =
		(int)syscall_handler & 0xffff;
	idt[128].segment_selector = kernel_cs;
	idt[128].reserved0 = 0;
	idt[128].gate_type = gate_type_32bit_int;
	idt[128].reserved1 = 0;
	idt[128].dpl = 0;
	idt[128].present = 1;
	idt[128].offset_hi =
		(int)syscall_handler >> 16;

	idt_descriptor.size = sizeof idt - 1;
	idt_descriptor.offset = (long)idt;

	asm volatile("lidt %0" : : "m"(idt_descriptor));
	asm volatile("sti");
}
