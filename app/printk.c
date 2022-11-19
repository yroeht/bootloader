#include "../lib/print.h"

int main(void)
{
	char *msg = "Testing printk from app\r\n";
	printk("%s", msg);
	return 0;
}

