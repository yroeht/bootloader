init_drive:
	push %dx
	xor %ah, %ah # int
	int $0x13
	pop %dx
	ret

	# read one sector
read_sector:
	push %ax
	mov $0x0201, %ax
	int $0x13
	test %dh, %dh
	jz end
end:
	pop %ax
	ret

