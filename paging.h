#pragma once

union page_directory_entry
{
	struct
	{
		unsigned present : 1;
		unsigned write : 1;
		unsigned supervisor : 1;
		unsigned write_through : 1;
		unsigned cache_disable : 1;
		unsigned accessed : 1;
		unsigned available0 : 1;
		unsigned page_size_4M : 1;
		unsigned available : 3;
		unsigned page_attribute_table : 1;
		long address_bits_31_12 : 20;
	} __attribute__((packed));
	long bytes;
};

union page_table_entry
{
	struct
	{
		unsigned present : 1;
		unsigned write : 1;
		unsigned supervisor : 1;
		unsigned write_through : 1;
		unsigned cache_disable : 1;
		unsigned accessed : 1;
		unsigned dirty : 1;
		unsigned page_attribute : 1;
		unsigned global : 1;
		unsigned available : 3;
		long address_bits_31_12 : 20;
	} __attribute__((packed));
	long bytes;
};

void paging_setup(void);
void paging_dump(const char *);
