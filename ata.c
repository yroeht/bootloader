#include "ata.h"
#include "ports.h"

static const int words_per_sector = 256;

void ata_lba_read(long lba, char nb, short* addr)
{
	for (int n = 0; n < nb; ++n)
	{
		lba &= 0x0fffffff;
		outb(0x01f6, (lba >> 24) | 0xe0);
		outb(0x01f2, nb);
		outb(0x01f3, lba);
		outb(0x01f4, lba >> 8);
		outb(0x01f5, lba >> 16);

		outb(0x01f7, 0x20);

		char status = inb(0x01f7);
		while (!(status & 8))
			status = inb(0x01f7);
		for (int i = 0; i < words_per_sector; ++i)
			addr[i] = inw(0x01f0);
		addr += words_per_sector;
		++lba;
	}
}

void ata_lba_write(long lba, char nb, const short* addr)
{
	for (int n = 0; n < nb; ++n)
	{
		lba &= 0x0fffffff;
		outb(0x01f6, (lba >> 24) | 0xe0);
		outb(0x01f2, nb);
		outb(0x01f3, lba);
		outb(0x01f4, lba >> 8);
		outb(0x01f5, lba >> 16);

		outb(0x01f7, 0x30);

		char status = inb(0x01f7);
		while (!(status & 8))
			status = inb(0x01f7);
		for (int i = 0; i < words_per_sector; ++i)
			outw(0x01f0, addr[i]);
		addr += words_per_sector;
		lba++;
	}
}


