#include <string.h>
#include "testsuite.h"

char *strtok(char *str, const char *delims)
{
	static char *save = "";
	return strtok_r(str, delims, &save);
}

TESTSUITE {
	// TODO
}


