#pragma once

static inline void outb(short port, char b)
{
	asm volatile("outb %0, %1" : : "a"(b), "Nd"(port));
}

static inline void outw(short port, short b)
{
	asm volatile("outw %0, %1" : : "a"(b), "Nd"(port));
}

static inline char inb(short port)
{
	char ret = 0x11;
	asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

static inline short inw(short port)
{
	short ret = 0x12;
	asm volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}
