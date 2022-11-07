COMMON_FLAGS=-fno-pie -Wall -Wextra -pedantic -fno-stack-protector -g
CFLAGS=-m32 ${COMMON_FLAGS}
ASFLAGS=-m16 ${COMMON_FLAGS}
LDFLAGS=--nmagic -m elf_i386
BOOT_IMAGE=boot.img

C_SOURCE=kernel.c \
	 print.c \
	 idt.c \
	 isr.c \

ASM_SOURCE=boot.S \
	   pmode.S \
	   pic.S \

C_OBJ= $(C_SOURCE:.c=.o gdt.o isr.o)

ASM_OBJ= $(ASM_SOURCE:.S=.o)

all: clean ${BOOT_IMAGE}
	qemu-system-i386 -fda ${BOOT_IMAGE}

${BOOT_IMAGE}: ${ASM_OBJ} ${C_OBJ}
	ld -Tlinker.ld $? -o boot.elf ${LDFLAGS}
	objcopy -O binary boot.elf $@

gdt.o:gdt.c
	${CC} -m16 ${COMMON_FLAGS}   -c -o $@ $^

isr.o:isr.c
	${CC} ${CFLAGS} -mgeneral-regs-only    -c -o $@ $^

debug: clean ${BOOT_IMAGE}
	qemu-system-i386 -fda ${BOOT_IMAGE} -s -S

clean:
	rm -vf *.o ${BOOT_IMAGE}
