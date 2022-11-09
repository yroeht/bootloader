#include "ports.h"

static const int words_per_sector = 256;

void ata_lba_read(long lba, char nb, char* addr)
{
	lba &= 0x0fffffff;
	outb(0x01f6, (lba >> 24) | 0xe0);
	outb(0x01f2, nb);
	outb(0x01f3, lba);
	outb(0x01f4, lba >> 8);
	outb(0x01f5, lba >> 16);

	outb(0x01f7, 0x20);

	char status = inb(0x20);
	while (!(status & 8))
		continue;
	for (int i = 0; i < nb * words_per_sector; ++i)
		addr[i] = inb(0x01f0);
}

void ata_lba_write(long lba, char nb, const char* addr)
{
	lba &= 0x0fffffff;
	outb(0x01f6, (lba >> 24) | 0xe0);
	outb(0x01f2, nb);
	outb(0x01f3, lba);
	outb(0x01f4, lba >> 8);
	outb(0x01f5, lba >> 16);

	outb(0x01f7, 0x30);

	char status = inb(0x30);
	while (!(status & 8))
		continue;
	for (int i = 0; i < nb * words_per_sector; ++i)
		outb(0x01f0, addr[i]);
}


