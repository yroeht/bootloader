#include "isr.h"
#include "print.h"

__attribute__ ((interrupt))
void default_interrupt_handler(struct interrupt_frame *frame)
{
	(void) frame;
	print_string(__func__);
	print_string("\r\n");
}


