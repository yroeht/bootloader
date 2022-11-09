#pragma once

static inline void outb(short port, unsigned char b)
{
	asm volatile("outb %0, %1" : : "a"(b), "Nd"(port));
}

static inline char inb(short port)
{
	char ret;
	asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

