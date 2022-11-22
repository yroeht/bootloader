#include "fat.h"
#include "lib/print.h"
#include "ata.h"
#include "string.h"

#define SECTOR_SIZE 512

static void print_time(short arg)
{
	union fat_time time;
	time.w = arg;
	int hours = time.hours & 0x1f;
	int minutes = time.minutes & 0x3f;
	int seconds = (time.seconds & 0x1f) * 2;
	printk("%2.d:%2.d:%2.d", hours, minutes, seconds);
}

static void print_date(short arg)
{
	union fat_date date;
	date.w = arg;
	char *months[] =
	{
		"JAN", "FEB", "MAR", "APR",
		"MAY", "JUN", "JUL", "AUG",
		"SEP", "OCT", "NOV", "DEC"
	};
	int year = (date.year & 0x7f) + 1980;
	int month = date.month & 0xf;
	int day = date.day & 0x1f;
	printk("%d-%s-%d", day, months[month - 1], year);
}

static void print_dirent(const struct fat_directory *dir)
{
	printk("%.*s  %x ", sizeof dir->filename, dir->filename, dir->attributes);
	print_time(dir->time_created);
	printk(" ");
	print_date(dir->date_created);
	printk(" %x %d B", dir->cluster_lo, dir->file_size);
}

static char bootrecord_bytes[SECTOR_SIZE];

static struct boot_record *br;

static void load_bootrecord(void)
{
	unsigned partition_start_sector = FS_OFFSET / SECTOR_SIZE;

	ata_lba_read(partition_start_sector, 1, (void*)bootrecord_bytes);
	br = (struct boot_record *)bootrecord_bytes;
}

static unsigned char fat[SECTOR_SIZE];

static void load_fat(void)
{
	unsigned partition_start_sector = FS_OFFSET / SECTOR_SIZE;
	unsigned fat_base = partition_start_sector + br->number_of_reserved_sectors;

	ata_lba_read(fat_base, 1, (void*)fat);
}

static unsigned char dir_bytes[SECTOR_SIZE];

static struct fat_directory *dir = (struct fat_directory*)dir_bytes;

static void load_rootdir(void)
{
	unsigned partition_start_sector = FS_OFFSET / SECTOR_SIZE;
	unsigned fat_base = partition_start_sector + br->number_of_reserved_sectors;
	unsigned first_fat_sector = fat_base
		+ br->number_of_file_allocation_tables * br->number_of_sectors_per_fat;

	ata_lba_read(first_fat_sector, 1, (void*)dir);
}

void fs_init(void)
{
	load_bootrecord();
	load_fat();
	load_rootdir();
}

void print_dir(const char *param)
{
	(void)param;
	for (int i = 0; i < br->number_of_root_directory_entries; ++i)
	{
		struct fat_directory *dirp = dir + i;
		if (!*dirp->filename)
			break;
		print_dirent(dirp);
		printk("\r\n");
	}
}

static struct fat_directory *find_file(const char *filename)
{
	for (int i = 0; i < br->number_of_root_directory_entries; ++i)
	{
		struct fat_directory *dirp = dir + i;
		if (!*dirp->filename)
			break;
		if (!strncmp(dirp->filename, filename, sizeof dirp->filename))
			return dirp;
	}
	return 0;
}

static void load(struct fat_directory *file, void *dst)
{
	unsigned active_cluster = file->cluster_lo;
	unsigned fat_offset = active_cluster + (active_cluster / 2);
	unsigned int ent_offset = fat_offset % SECTOR_SIZE;
	unsigned short table_value = *(unsigned short*)&fat[ent_offset];

	if(active_cluster & 0x0001)
		table_value = table_value >> 4;
	else
		table_value = table_value & 0x0FFF;

	unsigned partition_start_sector = FS_OFFSET / SECTOR_SIZE;
	unsigned fat_base = partition_start_sector + br->number_of_reserved_sectors;
	unsigned first_fat_sector = fat_base
		+ br->number_of_file_allocation_tables * br->number_of_sectors_per_fat;
	unsigned file_sector = first_fat_sector
		+ (br->number_of_root_directory_entries
			* sizeof (struct fat_directory))
			/ br->number_of_bytes_per_sector
		+ (active_cluster - 2)
			* br->number_of_sectors_per_cluster;

	ata_lba_read(file_sector, 1, (void*)dst);
}

void print_file(const char *filename)
{
	unsigned char filecontent[SECTOR_SIZE];
	struct fat_directory *file = find_file(filename);

	if (!file)
	{
		printk("file <%s> not found.\r\n", filename);
		return;
	}
	load(file, filecontent);
	for (int i = 0; i < file->file_size; ++i)
		printk("%c", filecontent[i]);
	printk("\r\n");
}

void change_dir(const char *dirname)
{
	struct fat_directory *file = find_file(dirname);
	if (!file)
	{
		printk("directory <%s> not found.\r\n", dirname);
		return;
	}
	load(file, dir_bytes);
}
