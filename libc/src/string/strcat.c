// Copyright (C) 2015 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include <string.h>

char *strcat(char *dest, const char *src)
{
	char *out = dest;
	while (*dest) dest++;
	while ((*dest++ = *src++)) {}
	return out;
}

#ifdef TESTSUITE
TESTSUITE(strcat) {
	char buf[64];
	strcpy(buf, "hello ");
	CHECK(buf == strcat(buf, "my name is "));
	CHECK_STR(buf, "hello my name is ", 64);
	CHECK(buf == strcat(buf, "inigo montoya"));
	CHECK_STR(buf, "hello my name is inigo montoya", 64);
}
#endif


