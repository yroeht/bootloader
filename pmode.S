	.code16
	.section .bits16.code
	.globl pmode_switch
pmode_switch:
	# save the cursor position to resume there in 32 bits
	call cursor_save
	call init_gdt

	.code32
	.globl start32
start32:
	mov kernel_ds, %eax
	mov %eax, %ds
	mov %eax, %es
	mov %eax, %fs
	mov %eax, %gs
	mov %eax, %ss
	mov $0x00ffffff, %ebp
	mov %ebp, %esp
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
	mov $80, %cl
	mul %cl

	#offset by the column value
	xor %dh, %dh
	add %dx, %ax

	#save calculated offset
	mov %ax, (framebuffer_idx)
	pop %cx
	pop %ax

	call invisible_cursor

	ret
