all:
	gcc -c boot.S -o boot.o -m32
	gcc -c print32.S -o print32.o -m32
	gcc -c pmode.S -o pmode.o -m32
	gcc -c kernel.c -o kernel.o -m32 -fno-pie
	ld boot.o pmode.o kernel.o print32.o -o boot.img --oformat binary -Ttext 0x7c00 --nmagic -m elf_i386
	qemu-system-i386 -fda boot.img
