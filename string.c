#include "string.h"

int strncmp(const char *a, const char *b, int n)
{
	for (; n-- && a && b; ++a, ++b)
	{
		if (!a)
			return -1;
		if (!b)
			return 1;
		if (*a < *b)
			return -1;
		if (*a > *b)
			return 1;
	}
	return 0;
}

int strlen(const char *s)
{
	int len = 1;
	while (*s++)
		++len;
	return len - 1;
}
