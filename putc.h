#pragma once

#include "stdint.h"

void putc(char c);
void reset_colorcode(void);

enum color
{
	color_black = 0,
	color_blue = 1,
	color_green = 2,
	color_cyan = 3,
	color_red = 4,
	color_magenta = 5,
	color_brown = 6,
	color_white = 7,
};

union colorcode
{
	struct
	{
		u8 fg : 3;
		u8 intensity : 1;
		u8 bg : 3;
		u8 blink : 1;
	} __attribute__((packed));
	char byte;
};

extern union colorcode framebuffer_color;
