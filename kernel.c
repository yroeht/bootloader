#include "ata.h"
#include "idt.h"
#include "pic.h"
#include "print.h"
#include "fat.h"

extern short number_extra_sectors;
extern long __start_of_mem;

#define MAGIC_CHECK 0x12345678

long magic_check __attribute__((section(".end_of_elf"))) = MAGIC_CHECK;

short sector[256];

void kernel_entry(void)
{
	reset_colorcode();
	printk("Welcome to 32 bits Protected Mode!\r\n");
	printk("Loaded %d extra sectors after bootsector.\r\n",
			number_extra_sectors);
	if (magic_check != MAGIC_CHECK)
	{
		printk("Check of magic number failed, "
				"address %p read %p (expected %p) "
				"end of ELF is not loaded properly.\r\n",
				&magic_check, magic_check, MAGIC_CHECK);
		for (;;)
			continue;
	}
	printk("Magic check validated, ELF loaded until %p\r\n", &magic_check);
	pic_init();
	init_idt();
	asm volatile ("int $0x1");
	asm volatile ("int $0x81");

	ata_lba_read(0, 1, sector);
	printk("Magic according to ATA read of sector 0: %p\r\n", sector[255] & 0xffff);

	struct boot_record *br = (struct boot_record*)(FS_OFFSET + (char*)&__start_of_mem);

	const int sector_size = 512;

	unsigned partition_start_sector = FS_OFFSET / sector_size;

	unsigned char fat[sector_size];
	unsigned fat_base = partition_start_sector + br->number_of_reserved_sectors;
	ata_lba_read(fat_base, 1, (void*)fat);

	unsigned char rootdir[sector_size];
	struct fat_directory *fat_dirp = (struct fat_directory*)rootdir;
	unsigned first_fat_sector = fat_base
		+ br->number_of_file_allocation_tables * br->number_of_sectors_per_fat;
	ata_lba_read(first_fat_sector, 1, (void*)fat_dirp);
	print_dir(fat_dirp);
	printk("\r\n");

	unsigned active_cluster = fat_dirp->cluster_lo;
	unsigned fat_offset = active_cluster + (active_cluster / 2);
	unsigned int ent_offset = fat_offset % sector_size;
	unsigned short table_value = *(unsigned short*)&fat[ent_offset];
	if(active_cluster & 0x0001)
		table_value = table_value >> 4;
	else
		table_value = table_value & 0x0FFF;

	unsigned char filecontent[sector_size];
	unsigned file_sector = first_fat_sector
		+ (br->number_of_root_directory_entries
			* sizeof (struct fat_directory))
			/ br->number_of_bytes_per_sector
		+ (active_cluster - 2)
			* br->number_of_sectors_per_cluster
			* br->number_of_bytes_per_sector;
	ata_lba_read(file_sector, 1, (void*)filecontent);
	for (int i = 0; i < fat_dirp->file_size; ++i)
		printk("%c", filecontent[i]);
	printk("\r\n");


	for (;;)
		continue;
}
