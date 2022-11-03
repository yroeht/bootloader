CFLAGS=-m32 -fno-pie -Wall -Wextra -pedantic
ASFLAGS=${CFLAGS}
LDFLAGS=--oformat binary -Ttext 0x7c00 --nmagic -m elf_i386
BOOT_IMAGE=boot.img

C_SOURCE=kernel.c

ASM_SOURCE=boot.S \
	   print32.S \
	   pmode.S \

C_OBJ= $(C_SOURCE:.c=.o)

ASM_OBJ= $(ASM_SOURCE:.S=.o)

all: ${BOOT_IMAGE}
	qemu-system-i386 -fda ${BOOT_IMAGE}

${BOOT_IMAGE}: ${ASM_OBJ} ${C_OBJ}
	ld $? -o $@ ${LDFLAGS}

debug: ${BOOT_IMAGE}
	qemu-system-i386 -fda ${BOOT_IMAGE} -s -S

clean:
	rm -vf *.o ${BOOT_IMAGE}
