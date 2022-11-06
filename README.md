# Kernel and bootloader

Toy kernel and bootloader in x86 assembly and C.

## Steps

0. Load the bootsector
1. Print to TTY
2. Load some additional sectors
3. Setup Global Descriptor Table and enter Protected Mode
4. Print to framebuffer
5. Setup Interrupt Descriptor Table
6. Configure 8259 PIC
7. Configure 8042 PS/2
8. ...
9. Load an ELF and execute it (TODO)
10. Load another ELF and execute it too (TODO)
