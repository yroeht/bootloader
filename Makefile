NUM_SECTORS=100
SECTOR_SZ=512
FS_OFFSET=$$((${NUM_SECTORS} * ${SECTOR_SZ}))
COMMON_FLAGS=-fno-pie -Wall -Wextra -pedantic -fno-stack-protector -g -DFS_OFFSET=${FS_OFFSET}
CFLAGS=-m32 ${COMMON_FLAGS}
ASFLAGS=-m16 ${COMMON_FLAGS}
LDFLAGS=--nmagic -m elf_i386
BOOT_IMAGE=boot.img
ELF=boot.elf
FS_IMAGE=fs.img
EMU=qemu-system-i386

C_SOURCE=kernel.c \
	 print.c \
	 idt.c \
	 isr.c \
	 pic.c \
	 ata.c \
	 fat.c \

ASM_SOURCE=boot.S \
	   pmode.S \

C_OBJ= $(C_SOURCE:.c=.o gdt.o isr.o)

ASM_OBJ= $(ASM_SOURCE:.S=.o)

all: app run

app:
	make -C app

run: ${BOOT_IMAGE}
	${EMU} -hda ${BOOT_IMAGE}

${BOOT_IMAGE}: ${ELF}
	cp ${ELF} $@
	truncate -s ${FS_OFFSET} $@
	dd if=${FS_IMAGE} bs=${SECTOR_SZ} >>$@

${ELF}: ${ASM_OBJ} ${C_OBJ}
	ld -Tlinker.ld $? -o ${ELF} ${LDFLAGS}
	objcopy -O binary ${ELF} $@

gdt.o:gdt.c
	${CC} -m16 ${COMMON_FLAGS}   -c -o $@ $^

isr.o:isr.c
	${CC} ${CFLAGS} -mgeneral-regs-only    -c -o $@ $^

debug: ${BOOT_IMAGE}
	${EMU} -fda ${BOOT_IMAGE} -s -S

clean:
	make -C app $@
	rm -vf *.o ${BOOT_IMAGE}

${FS_IMAGE}: app
	rm -f ${FS_IMAGE}
	mkfs.fat -F12 -C ${FS_IMAGE} 64
	mcopy -i ${FS_IMAGE} fs/* ::
	mcopy -i ${FS_IMAGE} app/*.elf ::
	mdir -i ${FS_IMAGE} ::

.PHONY: ${FS_IMAGE} app
