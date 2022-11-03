	.code32
fb:
	.long 0xb8000
	.globl fb_offset
fb_offset:
	.long 0
columns:
	.long 80
print_char32:
	push %eax
	push %ebx
	push %edx
	mov (fb_offset), %edx

	mov $'\r', %bl
	cmp %al, %bl
	jne print_char32_next
	push %eax
	mov %edx, %eax
	xor %edx, %edx
	mov (columns), %ebx
	idiv %ebx
	mov %eax, %edx
	mov (columns), %eax
	imul %eax, %edx
	pop %eax
	jmp print_char32_exit
print_char32_next:
	mov $'\n', %bl
	cmp %al, %bl
	jne print_char32_body
	add (columns), %edx
	jmp print_char32_exit

print_char32_body:
	mov (fb), %ebx
	movb $0x0f, %ah
	xchg %ebx, %esi
	movw %ax, (%esi, %edx, 2)
	xchg %ebx, %esi
	inc %edx
print_char32_exit:
	mov %edx, (fb_offset)
	pop %edx
	pop %ebx
	pop %eax
	ret
	.globl print_str32
print_str32:
	push %ebp
	mov %esp, %ebp
	push %esi
	push %eax
	push %ecx
	mov 8(%ebp), %esi
print_str32_loop:
	mov $1, %ecx
	lodsb
	test %al, %al
	jz print_str32_exit
	call print_char32
	jmp print_str32_loop
print_str32_exit:
	pop %ecx
	pop %eax
	pop %esi
	mov %ebp, %esp
	pop %ebp
	ret

	.globl print_hex32
print_hex32:
	push %ebp
	mov %esp, %ebp
	push %ecx
	mov 8(%ebp), %ecx
	push %eax
	push %ebx
	push %esi

	push $hex_prefix
	call print_str32
	add $4, %esp

	mov $hex, %ebx

	mov %ecx, %esi
	and $0xf0000000, %esi
	shr $28, %esi
	mov (%ebx,%esi,1), %al
	call print_char32

	mov %ecx, %esi
	and $0xf000000, %esi
	shr $24, %esi
	mov (%ebx,%esi,1), %al
	call print_char32

	mov %ecx, %esi
	and $0xf00000, %esi
	shr $20, %esi
	mov (%ebx,%esi,1), %al
	call print_char32

	mov %ecx, %esi
	and $0xf0000, %esi
	shr $16, %esi
	mov (%ebx,%esi,1), %al
	call print_char32

	mov %ecx, %esi
	and $0xf000, %esi
	shr $12, %esi
	mov (%ebx,%esi,1), %al
	call print_char32

	mov %ecx, %esi
	and $0xf00, %esi
	shr $8, %esi
	mov (%ebx,%esi,1), %al
	call print_char32

	mov %ecx, %esi
	and $0xf0, %esi
	shr $4, %esi
	mov (%ebx,%esi,1), %al
	call print_char32

	mov %ecx, %esi
	and $0xf, %esi
	mov (%ebx,%esi,1), %al
	call print_char32

	pop %esi
	pop %ecx
	pop %ebx
	pop %eax
	mov %ebp, %esp
	pop %ebp
	ret
