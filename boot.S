	.code16
	.section .boot
	.globl _start
_start:
	cld

	xor %ax, %ax
	mov %ax, %es
	mov %ax, %ds
	mov %ax, %ss

	mov %dl, (drive_num)

	call clear
	mov $banner_msg, %si
	call print_string16

	call init_drive
	jc error

	# prepare to read drive
	mov (drive_num), %dl
	mov $0x0002, %cx # sector number 6 bits
	mov (number_extra_sectors), %ax
	mov $0x7c00, %bx
	add $512, %bx

read_loop:
	push %ax
	call read_sector
	jc error
	pop %ax
	dec %ax
	jz end_read
	add $512, %bx
	jnc skip_inc_es
	push %ax
	mov %es, %ax
	add $0x1000, %ax
	mov %ax, %es
	pop %ax
skip_inc_es:
	inc %cl
	# TODO: acquire disk geometry
	cmp $0x25, %cl
	jne read_loop
	mov $1, %cl
	inc %ch
	jmp read_loop
end_read:

	call pmode_switch

error:
	mov $error_msg, %si
	call print_string16
	call newline
error_loop:
	jmp error_loop

	#include "print16.S"
	#include "drive.S"

error_msg:
	.string "Error...\r\n"
	.globl banner_msg
banner_msg:
	.string "Welcome to 16 bits Real Mode!\r\n"

	.global number_extra_sectors
number_extra_sectors:
	.short 150
	.globl drive_num
drive_num:
	.byte
	.org 510, 0
	.short 0xaa55

