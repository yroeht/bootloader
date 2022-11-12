#include <stdarg.h>
#include "print.h"

int framebuffer_idx __attribute__((section(".bits16.data")));
union colorcode framebuffer_color;

void reset_colorcode(void)
{
	framebuffer_color.fg = color_white;
	framebuffer_color.intensity = 1;
	framebuffer_color.bg = color_black;
	framebuffer_color.blink = 0;
}

void print_char(char c)
{
	static short * const framebuffer = (short *) 0xb8000;
	const int columns = 80;
	const int rows = 24;

	if (!c)
		return;

	if ('\r' == c) {
		framebuffer_idx -= framebuffer_idx % columns;
		return;
	}
	if ('\n' == c) {
		framebuffer_idx += columns;
		return;
	}
	framebuffer[framebuffer_idx] = (framebuffer_color.byte << 8) | c;

	// if this newline is out of screen, scroll
	if (framebuffer_idx / columns >= rows) {
		for (int i = 0; i < framebuffer_idx; ++i) {
			framebuffer[i] = framebuffer[i + columns];
		}
		framebuffer_idx -= columns;
	}
	framebuffer_idx++;
}

static void print_string(const char *s)
{
	while (*s)
		print_char(*s++);
}

static void print_hex(int x)
{
	print_string("0x");
	const int nibbles = 2 * sizeof x;
	for (int i = nibbles - 1; i >= 0; --i)
		print_char("0123456789abcdef"[((x >> (4 * i)) & 0xf)]);
}

static void print_base(int x, unsigned b)
{
	char stack[33];
	char sym[62] =
		"0123456789"
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	if (b < 2 || b > sizeof sym)
		return;

	if (!x)
		print_char('0');

	for (unsigned i = 0; i < sizeof stack; ++i)
		stack[i] = 0;

	unsigned i;
	for (i = 1; x && i < sizeof stack; ++i, x /= b)
		stack[sizeof stack - i - 1] = sym[x % b];

	print_string(&stack[sizeof stack - i]);
}

void printk(const char *fmt, ...)
{
	va_list ap;
	int possible_arg = 0;
	va_start(ap, fmt);
	while (*fmt)
	{
		if (possible_arg)
		{
			if ('s' == *fmt)
				print_string(va_arg(ap, char *));
			if ('p' == *fmt)
				print_hex(va_arg(ap, int));
			if ('d' == *fmt)
				print_base(va_arg(ap, int), 10);
			if ('x' == *fmt)
				print_base(va_arg(ap, int), 16);
			if ('c' == *fmt)
				print_char(va_arg(ap, int));
			if ('%' == *fmt)
				print_char('%');
			possible_arg = 0;
			++fmt;
			continue;
		}
		if ('%' == *fmt)
			possible_arg = 1;
		else
			print_char(*fmt);
		++fmt;
	}
	va_end(ap);
}
