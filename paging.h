#pragma once

union page_directory_entry
{
	struct
	{
		char present : 1;
		char write : 1;
		char supervisor : 1;
		char write_through : 1;
		char cache_disable : 1;
		char accessed : 1;
		char available0 : 1;
		char page_size_4M : 1;
		char available : 3;
		char page_attribute_table : 1;
		long address_bits_31_12 : 20;
	} __attribute__((packed));
	long bytes;
};

union page_table_entry
{
	struct
	{
		char present : 1;
		char write : 1;
		char supervisor : 1;
		char write_through : 1;
		char cache_disable : 1;
		char accessed : 1;
		char dirty : 1;
		char page_attribute : 1;
		char global : 1;
		char available : 3;
		long address_bits_31_12 : 20;
	} __attribute__((packed));
	long bytes;
};

void paging_setup(void);
