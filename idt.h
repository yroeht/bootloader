#pragma once

#include "stdint.h"

void init_idt(void);

struct gate_descriptor
{
	short offset_lo;
	short segment_selector;
	char reserved0;
	struct
	{
		u8 gate_type : 4;
		u8 reserved1 : 1;
		u8 dpl : 2;
		u8 present : 1;
	} __attribute__((packed));
	short offset_hi;
} __attribute__((packed));

enum gate_type
{
	gate_type_task_gate = 0x5,
	gate_type_16bit_int = 0x6,
	gate_type_16bit_trap = 0x7,
	gate_type_32bit_int = 0xe,
	gate_type_32bit_trap = 0xf,
};

struct idt_descriptor
{
	short size;
	long offset;
} __attribute__((packed));
