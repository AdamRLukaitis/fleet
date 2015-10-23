#include <string.h>
#include "internal/testsuite.h"

char *strchr(const char *str, int ch)
{
	while (*str && *str != ch) {
		str++;
	}
	return (*str == ch) ? (char*)str : NULL;
}

TESTSUITE {
	static const char lower[] = "abcdefghijklmnopqrstuvwxyz";
	CHECK(&lower[0] == strchr(lower, 'a'));
	CHECK(&lower[3] == strchr(lower, 'd'));
	char buf[] = "abcabcabc";
	CHECK(&buf[1] == strchr(buf, 'b'));
	CHECK(&lower[25] == strchr(lower, 'z'));
	CHECK(0 == strchr(lower, 'A'));
	CHECK(&lower[26] == strchr(lower, '\0'));
	CHECK(0 == strchr("", 'X'));
	char dummy[] = "";
	CHECK(dummy == strchr(dummy, '\0'));
}


