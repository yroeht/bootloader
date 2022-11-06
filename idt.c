#include "gdt.h"
#include "idt.h"
#include "isr.h"

static struct gate_descriptor idt[256];

static struct idt_descriptor idt_descriptor;

void init_idt(void)
{

#define DEFAULT_ISR(n) \
	idt[n].offset_lo = \
	     (int)interrupt_handler##n & 0xffff; \
	idt[n].segment_selector = \
	     kernel_code * sizeof (union gdt_entry); \
	idt[n].reserved0 = 0; \
	idt[n].gate_type = gate_type_32bit_int; \
	idt[n].reserved1 = 0; \
	idt[n].dpl = 0; \
	idt[n].present = 1; \
	idt[n].offset_hi = \
		(int)interrupt_handler##n >> 16; \

#include "default-isr-nums.inc"

#undef DEFAULT_ISR

	idt_descriptor.size = sizeof (struct idt_descriptor)
		* sizeof idt - 1;
	idt_descriptor.offset = (long)idt;

	asm volatile("lidt %0" : : "m"(idt_descriptor));
	asm volatile("sti");
}
