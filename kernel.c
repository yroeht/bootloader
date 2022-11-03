extern void print_str32(const char *str);
extern void print_hex32(const int x);
extern char number_extra_sectors;

void kernel_entry(void)
{
	print_str32("Hello from kernel_entry!\r\n");
	print_str32("Loaded ");
	print_hex32(number_extra_sectors);
	print_str32(" extra sectors after bootsector.\r\n");
	for (;;)
		continue;
}
