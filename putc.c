#include "putc.h"

int framebuffer_idx __attribute__((section(".bits16.data")));
union colorcode framebuffer_color;

static const int columns = 80;
static const int rows = 24;
static short * const framebuffer = (short *) 0xb8000;

void reset_colorcode(void)
{
	framebuffer_color.fg = color_white;
	framebuffer_color.intensity = 1;
	framebuffer_color.bg = color_black;
	framebuffer_color.blink = 0;
}

static void scroll(void)
{
	// if this newline is out of screen, scroll
	if (framebuffer_idx / columns <= rows)
		return;
	for (int i = 0; i < columns * rows; ++i)
		framebuffer[i] = framebuffer[i + columns];
	for (int i = 0; i < columns; ++i)
		framebuffer[rows * columns + i] =
			(framebuffer_color.byte << 8) | ' ';
	framebuffer_idx -= columns;
}

void putc(char c)
{
	if (!c)
		return;

	if ('\r' == c) {
		framebuffer_idx -= framebuffer_idx % columns;
		return;
	}
	if ('\n' == c) {
		framebuffer_idx += columns;
		scroll();
		return;
	}

	scroll();
	framebuffer[framebuffer_idx] = (framebuffer_color.byte << 8) | c;
	framebuffer_idx++;
}

void move_cursor(int n)
{
	framebuffer_idx += n;
}
