#include "fat.h"
#include "print.h"

static void print_time(short arg)
{
	union fat_time time;
	time.w = arg;
	int hours = time.hours & 0x1f;
	int minutes = time.minutes & 0x3f;
	int seconds = (time.seconds & 0x1f) * 2;
	printk("%d:%d:%d", hours, minutes, seconds);
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

void print_dir(const struct fat_directory *dir)
{
	printk("%.*s  %x ", sizeof dir->filename, dir->filename, dir->attributes);
	print_time(dir->time_created);
	printk(" ");
	print_date(dir->date_created);
	printk(" %x %d B", dir->cluster_lo, dir->file_size);
}
