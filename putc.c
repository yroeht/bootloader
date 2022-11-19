#include "putc.h"

int framebuffer_idx __attribute__((section(".bits16.data")));
union colorcode framebuffer_color;

void reset_colorcode(void)
{
	framebuffer_color.fg = color_white;
	framebuffer_color.intensity = 1;
	framebuffer_color.bg = color_black;
	framebuffer_color.blink = 0;
}

void putc(char c)
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

