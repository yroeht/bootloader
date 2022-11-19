#include <stdarg.h>
#include "print.h"

static void putc(char c)
{
	asm volatile("push %0" : : "m"(c));
	asm volatile("push $0");
	asm volatile("int $0x80");
	asm volatile("pop %%eax" : : : "eax");
	asm volatile("pop %%eax" : : : "eax");
}

static void print_string(const char *s, int min, int max)
{
	while (*s && max--)
	{
		putc(*s++);
		min--;
	}
	while (min-- > 0)
		putc(' ');
}

static void print_base(int x, unsigned b, int min, int max)
{
	char stack[34];
	char sym[62] =
		"0123456789"
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char negative = x < 0;
	if (negative)
		x = -x;

	if (b < 2 || b > sizeof sym)
		return;

	for (unsigned i = 0; i < sizeof stack; ++i)
		stack[i] = 0;

	for (int i = 0; i < min; ++i)
		stack[sizeof stack - 2 - i] = sym[0];

	unsigned i;
	for (i = 1; max-- && x && i < sizeof stack; ++i, x /= b)
		stack[sizeof stack - i - 1] = sym[x % b];

	if (i < (unsigned)min + 1)
		i = min + 1;

	if (negative)
		stack[sizeof stack - ++i] = '-';

	print_string(&stack[sizeof stack - i], min, sizeof stack);
}

static void print_hex(int x)
{
	putc('0');
	putc('x');
	print_base(x, 16, 8, 8);
}

void printk(const char *fmt, ...)
{
	va_list ap;
	int possible_arg = 0;
	int reading_min = 0;
	int max;
	int min;
	va_start(ap, fmt);
	while (*fmt)
	{
		if (possible_arg && '.' == *fmt)
			reading_min = 0;
		if (possible_arg && '*' == *fmt)
		{
			if (reading_min)
				min = va_arg(ap, int);
			else
				max = va_arg(ap, int);
		}
		if (possible_arg)
		{
			if ('s' == *fmt)
				print_string(va_arg(ap, char *), min, max ? max : 1000000);
			else if ('p' == *fmt)
				print_hex(va_arg(ap, int));
			else if ('d' == *fmt)
				print_base(va_arg(ap, int), 10, min ? min : 1, max ? max : 32);
			else if ('x' == *fmt)
				print_base(va_arg(ap, int), 16, min ? min : 1, max ? max : 32);
			else if ('c' == *fmt)
				putc(va_arg(ap, int));
			else if ('%' == *fmt)
				putc('%');
			else if ('0' <= *fmt && *fmt <= '9')
			{
				if (reading_min)
					min = min * 10 + *fmt - '0';
				else
					max = max * 10 + *fmt - '0';
				++fmt;
				continue;
			}
			else
			{
				++fmt;
				continue;
			}
			possible_arg = 0;
			++fmt;
			continue;
		}
		if ('%' == *fmt)
		{
			reading_min = 1;
			possible_arg = 1;
			max = 0;
			min = 0;
		}
		else
			putc(*fmt);
		++fmt;
	}
	va_end(ap);
}
