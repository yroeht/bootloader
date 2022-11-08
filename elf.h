#pragma once

struct elf_header
{
	long magic;
	char bits32_or_64;
	char endianness;
	char elf_header_version;
	char os_abi;
	long long unused;
	short relocatable_or_executable;
	short instruction_set;
	long elf_version;
	long program_entry;
	long phdr_position;
	long shdr_position;
	long flags;
	short header_size;
	short phdr_entry_sz;
	short phdr_entry_num;
	short shdr_entry_sz;
	short shdr_entry_num;
	short shdr_names_index;
} __attribute__((packed));

struct program_header
{
	long type;
	long p_offset;
	long p_vaddr;
	long undefined;
	long p_filesz;
	long p_memsz;
	long flags;
	long alignment;
} __attribute__((packed));
