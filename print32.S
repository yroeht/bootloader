fb:
	.long 0xb8000
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
	mov 4(%esp), %esi
	push %eax
	push %ecx
	mov $1, %ecx
print_str32_loop:
	lodsb
	test %al, %al
	jz print_str32_exit
	call print_char32
	jmp print_str32_loop
print_str32_exit:
	pop %ecx
	pop %eax
	ret
