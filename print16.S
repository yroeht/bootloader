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

	.globl cursor_get
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

clear:
	push %ax
	push %cx
	push %dx
	xor %al, %al
	xor %cx, %cx
	mov $24, %dh
	mov $80, %dl
	mov $7, %bh
	mov $7, %ah
	int $0x10
	xor %dx, %dx
	call cursor_set
	pop %dx
	pop %cx
	pop %ax
	ret

	.globl invisible_cursor
invisible_cursor:
	push %ax
	push %cx
	mov $0x2607, %cx
	mov $1, %ah
	int $0x10
	pop %cx
	pop %ax
	ret

	.globl hex_prefix
hex_prefix:
	.string "0x"

	.globl hex
hex:
	.string "0123456789abcdef"
itoa_hex:
	push %ax
	push %bx
	push %cx
	push %si
	mov %ax, %cx
	mov $hex_prefix, %si
	call print_string

	mov $hex, %bx

	mov %cx, %si
	and $0xf000, %si
	shr $12, %si
	mov (%bx,%si,1), %al
	call print_char

	mov %cx, %si
	and $0xf00, %si
	shr $8, %si
	mov (%bx,%si,1), %al
	call print_char

	mov %cx, %si
	and $0xf0, %si
	shr $4, %si
	mov (%bx,%si,1), %al
	call print_char

	mov %cx, %si
	and $0xf, %si
	mov (%bx,%si,1), %al
	call print_char

	pop %si
	pop %cx
	pop %bx
	pop %ax
	ret
