COMMON_FLAGS=-fno-pie -Wall -Wextra -pedantic
CFLAGS=-m32 ${COMMON_FLAGS}
ASFLAGS=${CFLAGS}
LDFLAGS=--oformat binary -Ttext 0x7c00 --nmagic -m elf_i386
BOOT_IMAGE=boot.img

C_SOURCE=kernel.c \
	 print.c \

ASM_SOURCE=boot.S \
	   pmode.S \

C_OBJ= $(C_SOURCE:.c=.o gdt.o)

ASM_OBJ= $(ASM_SOURCE:.S=.o)

all: clean ${BOOT_IMAGE}
	qemu-system-i386 -fda ${BOOT_IMAGE}

${BOOT_IMAGE}: ${ASM_OBJ} ${C_OBJ}
	ld $? -o $@ ${LDFLAGS}

gdt.o:gdt.c
	${CC} -m16 ${COMMON_FLAGS}   -c -o $@ $^

debug: ${BOOT_IMAGE}
	qemu-system-i386 -fda ${BOOT_IMAGE} -s -S

clean:
	rm -vf *.o ${BOOT_IMAGE}
