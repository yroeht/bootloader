all:
	gcc -c boot.S -o boot.o -m32
	gcc -c kernel.c -o kernel.o -m32 -fno-pie
	ld boot.o kernel.o -o boot.img --oformat binary -Ttext 0x7c00 --nmagic -m elf_i386
	qemu-system-i386 -fda boot.img
