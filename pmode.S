	.code16
	.globl pmode_switch
pmode_switch:
	# save the cursor position to resume there in 32 bits
	call cursor_save

	call init_gdt
	.code32
start32_aux:
	ret
	.globl start32
start32:
	call start32_aux
	call kernel_entry
loop:
	jmp loop

	.code16
cursor_save:
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
	mov %ax, (framebuffer_idx)
	pop %cx
	pop %ax

	call invisible_cursor

	ret

