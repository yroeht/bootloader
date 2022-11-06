#include "isr.h"
#include "print.h"

extern void pic_ack(void);

#define ISR(n, str, has_error, is_excn)                        \
__attribute__ ((interrupt))                                    \
void interrupt_handler##n(struct interrupt_frame *frame)       \
{                                                              \
	(void) frame;                                          \
	if (has_error)                                         \
		printk("%s err=%p\r\n", str, frame->err);      \
	else                                                   \
		printk("%s\r\n", str);                         \
	asm volatile("mov $0x20, %%al": : : "al");             \
	asm volatile("outb %%al, $0x20": : : "al");            \
	asm volatile("outb %%al, $0xa0": : : "al");            \
}                                                              \

#include "isr-list.inc"

#undef ISR

__attribute__((interrupt))
void keyboard_handler(struct interrupt_frame *frame)
{
	(void) frame;
	int key = 0;
	asm volatile("inb $0x60, %%al" : : : "al");
	asm volatile("mov %%al, %0" : "=m"(key));
	asm volatile("mov $0x20, %%al": : : "al");
	asm volatile("outb %%al, $0x20": : : "al");
	asm volatile("outb %%al, $0xa0": : : "al");
	printk("%s key code %p\r\n", __func__, key);
}
