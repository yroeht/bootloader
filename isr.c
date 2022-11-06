#include "isr.h"
#include "print.h"

extern void pic_ack(void);

#define ISR(n, str, has_error, is_excn)                        \
__attribute__ ((interrupt))                                    \
void interrupt_handler##n(struct interrupt_frame *frame)       \
{                                                              \
	(void) frame;                                          \
	printk("%s\r\n", str);                                 \
	asm volatile("mov $0x20, %%al": : : "al"); \
	asm volatile("outb %%al, $0x20": : : "al"); \
	asm volatile("outb %%al, $0xa0": : : "al"); \
}                                                              \

#include "isr-list.inc"

#undef ISR
