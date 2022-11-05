#include "gdt.h"
#include "idt.h"
#include "isr.h"

static struct gate_descriptor idt[256];

static struct idt_descriptor idt_descriptor;

void init_idt(void)
{
	for (unsigned i = 0; i < sizeof idt; ++i)
	{
		idt[i].offset_lo =
			(int)default_interrupt_handler & 0xffff;
		idt[i].segment_selector =
			kernel_code * sizeof (union gdt_entry);
		idt[i].reserved0 = 0;
		idt[i].gate_type = gate_type_32bit_int;
		idt[i].reserved1 = 0;
		idt[i].dpl = 0;
		idt[i].present = 1;
		idt[i].offset_hi =
			(int)default_interrupt_handler >> 16;
	}
	idt_descriptor.size = sizeof idt - 1;
	idt_descriptor.offset = (long)idt;

	asm volatile("lidt %0" : : "m"(idt_descriptor));
}
