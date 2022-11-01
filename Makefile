all:
	gcc -c boot.S -o boot.o
	ld boot.o -o boot --oformat binary -Ttext 0x7c00
	qemu-system-i386 -fda boot
