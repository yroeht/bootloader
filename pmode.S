GDT:
	.long 0
	.long 0
	.long 0x0000ffff
	.long 0x00cf9a00
	.long 0x0000ffff
	.long 0x00cf8200
GDT_descriptor:
	.word GDT_descriptor - GDT - 1
	.long GDT
pmode_switch:
	cli
	lgdt (GDT_descriptor)
	mov %cr0, %eax
	or $1, %al
	mov %eax, %cr0
	ljmp $0x08, $start32

	.code32
start32:
	mov $banner32_msg, %si
	call print_str32
	mov $banner32_msg, %si
	call print_str32
loop32:
	jmp loop32

banner32_msg:
	.string "Welcome to 32 bits Protected Mode!\r\n"

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
print_str32:
	push %eax
print_str32_loop:
	mov (%si), %al
	test %al, %al
	jz print_str32_exit
	call print_char32
	inc %si
	jmp print_str32_loop
print_str32_exit:
	pop %eax
	ret
