#include "isr.h"
#include "print.h"

extern void pic_ack(void);

__attribute__ ((interrupt))
void default_interrupt_handler(struct interrupt_frame *frame)
{
	(void) frame;
	print_string(__func__);
	print_string("\r\n");
	pic_ack();
}

#define DEFAULT_ISR(n)                                         \
__attribute__ ((interrupt)) \
void interrupt_handler##n(struct interrupt_frame *frame)       \
{                                                              \
	(void) frame;                                          \
	print_string(__func__);                                \
	print_string("\r\n");                                  \
	asm volatile("mov $0x20, %%al": : : "al"); \
	asm volatile("outb %%al, $0x20": : : "al"); \
	asm volatile("outb %%al, $0xa0": : : "al"); \
}                                                              \

#include "default-isr-nums.inc"

#undef DEFAULT_ISR
