#include "isr.h"
#include "pic.h"
#include "print.h"

__attribute__ ((interrupt))
void default_interrupt_handler(struct interrupt_frame *frame)
{
	(void) frame;
	printk("%s\r\n", __func__);
}

#define ISR(n, str, has_error, is_excn)                        \
__attribute__ ((interrupt))                                    \
void interrupt_handler##n(struct interrupt_frame *frame)       \
{                                                              \
	(void) frame;                                          \
	if (has_error)                                         \
		printk("%s err=%p\r\n", str, frame->err);      \
	else                                                   \
		printk("%s\r\n", str);                         \
	pic_ack();                                             \
}                                                              \


#include "isr-list.inc"

#undef ISR

static int shift_pressed;
void process_key(char k)
{
	if (shift_pressed)
	{
		if (k >= 'a' && k < 'z')
			k += 'A' - 'a';
		switch (k)
		{
			case '1': k = '!'; break;
			case '2': k = '@'; break;
			case '3': k = '#'; break;
			case '4': k = '$'; break;
			case '5': k = '!'; break;
			case '6': k = '^'; break;
			case '7': k = '&'; break;
			case '8': k = '*'; break;
			case '9': k = '('; break;
			case '0': k = ')'; break;
			case ';': k = ':'; break;
			case '\'': k = '"'; break;
			case ',': k = '<'; break;
			case '.': k = '>'; break;
			case '/': k = '?'; break;
			case '_': k = '-'; break;
			case '+': k = '='; break;
			case '{': k = '['; break;
			case '}': k = ']'; break;
			case '\\': k = '|'; break;
		}
	}
	print_char(k);
}

__attribute__((interrupt))
void keyboard_handler(struct interrupt_frame *frame)
{
	(void) frame;
	int keycode = 0;
	asm volatile("inb $0x60, %%al" : : : "al");
	asm volatile("mov %%al, %0" : "=m"(keycode));

	pic_ack();

	switch (keycode)
	{
		case 0x2a: shift_pressed = 1; return;
		case 0xaa: shift_pressed = 0; return;
	}
	// ignore non-modifier key release
	if (keycode & 0x80)
		return;
	switch (keycode & 0x3f)
	{
		case 0x1e: process_key('a'); return;
		case 0x30: process_key('b'); return;
		case 0x2e: process_key('c'); return;
		case 0x20: process_key('d'); return;
		case 0x12: process_key('e'); return;
		case 0x21: process_key('f'); return;
		case 0x22: process_key('g'); return;
		case 0x23: process_key('h'); return;
		case 0x17: process_key('i'); return;
		case 0x24: process_key('j'); return;
		case 0x25: process_key('k'); return;
		case 0x26: process_key('l'); return;
		case 0x32: process_key('m'); return;
		case 0x31: process_key('n'); return;
		case 0x18: process_key('o'); return;
		case 0x19: process_key('p'); return;
		case 0x10: process_key('q'); return;
		case 0x13: process_key('r'); return;
		case 0x1f: process_key('s'); return;
		case 0x14: process_key('t'); return;
		case 0x16: process_key('u'); return;
		case 0x2f: process_key('v'); return;
		case 0x11: process_key('w'); return;
		case 0x2d: process_key('x'); return;
		case 0x15: process_key('y'); return;
		case 0x2c: process_key('z'); return;
		case 0x1c: process_key('\r'); process_key('\n'); return;
		case 0x39: process_key(' '); return;
		case 0x02: process_key('1'); return;
		case 0x03: process_key('2'); return;
		case 0x04: process_key('3'); return;
		case 0x05: process_key('4'); return;
		case 0x06: process_key('5'); return;
		case 0x07: process_key('6'); return;
		case 0x08: process_key('7'); return;
		case 0x09: process_key('8'); return;
		case 0x0a: process_key('9'); return;
		case 0x0b: process_key('0'); return;
		case 0x27: process_key(';'); return;
		case 0x28: process_key('\''); return;
		case 0x33: process_key(','); return;
		case 0x34: process_key('.'); return;
		case 0x35: process_key('/'); return;
		case 0x0c: process_key('_'); return;
		case 0x0d: process_key('+'); return;
		case 0x1a: process_key('{'); return;
		case 0x1b: process_key('}'); return;
		case 0x2b: process_key('\\'); return;
	}
	printk("%s key code %p\r\n", __func__, keycode);
}
