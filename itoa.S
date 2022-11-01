hex_prefix:
	.string "0x"
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
