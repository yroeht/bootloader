COMMON_FLAGS=-fno-pie -Wall -Wextra -pedantic -fno-stack-protector -g
CFLAGS=-m32 ${COMMON_FLAGS}
LDFLAGS=--nmagic -m elf_i386
ELF=printk.elf

C_SOURCE=printk.c \

C_OBJ= $(C_SOURCE:.c=.o)

${ELF}: ${C_OBJ}
	ld -Tlinker.ld $? -o ${ELF} ${LDFLAGS}

all: ${ELF}

clean:
	rm -vf *.o *.elf
