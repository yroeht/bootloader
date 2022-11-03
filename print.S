print_char:
	push %ax
	push %bx
	push %dx
	xor %bl, %bl
	xor %dh, %dh
	mov $0xe, %ah
	int $0x10
	pop %dx
	pop %bx
	pop %ax
	ret

cursor_set:
	push %ax
	push %bx
	xor %bh, %bh
	mov $0x02, %ah
	int $0x10
	pop %bx
	pop %ax
	ret

cursor_get:
	push %ax
	push %bx
	push %cx
	xor %bh, %bh
	xor %cx, %cx
	mov $0x03, %ah
	int $0x10
	pop %cx
	pop %bx
	pop %ax
	ret

print_string:
	push %si
	push %ax
	push %cx
print_string_loop:
	# strlen is unknown so iterate byte by byte until null
	mov $1, %cx
	lodsb
	test %al, %al
	jz print_string_exit
	call print_char
	jmp print_string_loop
print_string_exit:
	pop %cx
	pop %ax
	pop %si
	ret

newline:
	mov $'\r', %al
	call print_char
	mov $'\n', %al
	call print_char
	ret

