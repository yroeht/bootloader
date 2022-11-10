#pragma once

void ata_lba_read(long lba, char nb, short* addr);
void ata_lba_write(long lba, char nb, const short* addr);
