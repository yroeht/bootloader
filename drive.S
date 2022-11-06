init_drive:
	push %dx
	mov (drive_num), %dl
	xor %ah, %ah # int
	int $0x13
	pop %dx
	ret

	# read one sector
read_sector:
	push %ax
	test %al, %al
	mov $0x0201, %ax
	int $0x13
	pop %ax
	ret

