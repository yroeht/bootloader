init_drive:
	push %dx
	mov (drive_num), %dl
	xor %ah, %ah # int
	int $0x13
	pop %dx
	ret

	# read %al sectors
read_sector:
	push %dx
	xor %dx, %dx
	xor %ah, %ah
	mov %dx, %es
	mov (drive_num), %dl
	mov $0x02, %ah # int
	mov $0x0002, %cx # cylinder and sector number
	int $0x13
	pop %dx
	ret

