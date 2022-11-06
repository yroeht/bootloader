#pragma once

#include "stdint.h"

union gdt_entry
{
	struct
	{
		u16 limit_low;
		u16 base_low;
		u8 base_mid;
		u8 accessed : 1;
		u8 rw : 1;
		u8 conforming_or_direction : 1;
		u8 executable : 1;
		u8 type : 1;
		u8 dpl : 2;
		u8 present : 1;
		u8 limit_hi : 4;
		u8 reserved : 1;
		u8 long_mode : 1;
		u8 size : 1;
		u8 granularity : 1;
		u8 base_hi;
	} __attribute__((packed));
	struct
	{
		long w1;
		long w2;
	};
};

enum segment_gdt_idx
{
	null,
	kernel_code,
	kernel_data,
	user_code,
	user_data,
	segment_gdt_idx_max,
};

enum segment_gdt_type
{
	system = 0,
	code_or_data = 1,
};

enum segment_gdt_direction
{
	expand_up = 0,
	expand_down = 1,
};

enum segment_gdt_granularity
{
	one_byte = 0,
	one_page = 1,
};

enum segment_gdt_size
{
	bits_16 = 0,
	bits_32 = 1,
};

struct gdt_descriptor
{
	short size;
	long address;
} __attribute__((__packed__));

extern const char kernel_ds;

