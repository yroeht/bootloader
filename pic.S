.code32
	.text
	.globl init_pic

init_pic:
	# icw1
	mov $0x11, %al
	outb %al, $0x20
	outb %al, $0xa0

	# icw2
	mov $0x40, %al
	outb %al, $0x21
	add $0x10, %al
	outb %al, $0xa1

	# icw3
	mov $0x01, %al
	outb %al, $0x21
	outb %al, $0xa1

	# icw4
	mov $0x01, %al
	outb %al, $0x21
	outb %al, $0xa1

	# ocw1
	mov $0xfd, %bl
	inb $0x21, %al
	or %bl, %al
	outb %al, $0x21

	ret

	.globl pic_ack
pic_ack:
	mov $0x20, %al
	outb %al, $0x20
	outb %al, $0xa0
	ret

