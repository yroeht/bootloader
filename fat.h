#pragma once

struct boot_record
{
	char jump_code[3];
	char oem_identifier[8];
	short number_of_bytes_per_sector;
	char number_of_sectors_per_cluster;
	short number_of_reserved_sectors;
	char number_of_file_allocation_tables;
	short number_of_root_directory_entries;
	short total_sectors;
	char media_descriptor_type;
	short number_of_sectors_per_fat;
	short number_of_sectors_per_track;
	short number_of_heads;
	long number_of_hidden_sectors;
	long large_sector_count;
	char drive_number;
	char reserved;
	char signature;
	long volume_id;
	char volume_label[11];
	char system_id[8];
} __attribute__((packed));

struct fat_directory
{
	char filename[11];
	char attributes;
	char reserved;
	char creation_time_sec_tens;
	short time_created;
	short date_created;
	short last_accessed_date;
	short cluster_hi;
	short last_mod_time;
	short last_mod_date;
	short cluster_lo;
	long file_size;
} __attribute__((packed));

union fat_time
{
	struct
	{
		short seconds : 5;
		short minutes : 6;
		short hours : 5;
	} __attribute__((packed));
	short w;
};

union fat_date
{
	struct
	{
		short day : 5;
		short month : 4;
		short year : 7;
	} __attribute__((packed));
	short w;
};

void fs_init(void);
void print_dir(const char *param);
void print_file(const char *filename);
void change_dir(const char *dirname);

struct fat_directory *find_starts_with(const char *);
