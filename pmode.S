	.code16
	.globl pmode_switch
pmode_switch:
	# save the cursor position to resume there in 32 bits
	call cursor_get
	push %ax
	push %cx

	# offset by the row value
	xor %ah, %ah
	mov %dh, %al
	mov $80, %cx
	mul %cx

	#offset by the column value
	xor %dh, %dh
	shl $1, %dl
	add %dx, %ax

	#save calculated offset
	mov %ax, (fb_offset)
	pop %cx
	pop %ax

	call invisible_cursor

	call init_gdt

	.code32
	.globl start32
start32:
	mov (kernel_cs), %eax
	mov %eax, %ds
	mov %eax, %ss
	mov %eax, %es
	mov %eax, %fs
	mov %eax, %gs
	call print_banner32
	call kernel_entry

print_banner32:
	push $banner32_msg
	call print_str32
	add $4, %esp
	ret

banner32_msg:
	.string "Welcome to 32 bits Protected Mode!\r\n"

