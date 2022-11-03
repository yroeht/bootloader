	.code16
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

	cli
	lgdt (GDT_descriptor)
	mov %cr0, %eax
	or $1, %al
	mov %eax, %cr0
	ljmp $0x08, $start32

	.code32
start32:
	call print_banner32
	call kernel_entry
loop32:
	jmp loop32

	.globl print_banner32
print_banner32:
	pusha
	push $banner32_msg
	call print_str32
	add $4, %esp
	popa
	ret

banner32_msg:
	.string "Welcome to 32 bits Protected Mode!\r\n"

