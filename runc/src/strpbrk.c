#include <string.h>
#include "testsuite.h"

char *strpbrk(const char *str, const char *set)
{
	while (*str) {
		for (const char *c = set; *c; c++) {
			if (*str == *c) return (char*)str;
		}
		str++;
	}
	return NULL;
}

TESTSUITE {
	// TODO
}
