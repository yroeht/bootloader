int main(void)
{
	char *msg = "Testing printk syscall\r\n";
	asm volatile("mov $0, %%eax" : : : "eax");
	asm volatile("mov %0, %%ebx" : : "m"(msg) : "ebx");
	asm volatile("int $0x80");
	return 0;
}
