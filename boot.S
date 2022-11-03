	.code16
	.globl _start

drive_num:
	.byte

	.text

_start:
	cld

	call clear

	mov %dl, (drive_num)
	mov $banner_msg, %si
	call print_string

	mov $bootsect_str, %si
	call print_string
	mov (mem_start), %si
	add $510, %si
	mov (%si), %ax
	call itoa_hex
	call newline

	call init_drive
	jc error
	mov (mem_start), %bx
	add $512, %bx
	xor %ax, %ax
	mov %ax, %es
	mov (number_extra_sectors), %al
	call read_sector
	jc error

	call pmode_switch

error:
	mov $error_msg, %si
	call print_string
error_loop:
	jmp error_loop

	#include "print16.S"
	#include "itoa.S"
	#include "drive.S"

banner_msg:
	.string "Welcome to 16 bits Real Mode!\r\n"
error_msg:
	.string "Error...\r\n"
bootsect_str:
	.string "Our first sector's magic number is "

number_extra_sectors:
	.byte 0x1
mem_start:
	.word 0x7c00

	.org 510, 0
	.short 0xaa55
	#include "pmode.S"
	#include "print32.S"
