// Copyright (C) 2015 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include <string.h>

int strcmp(const char *l, const char *r)
{
	while (*l && *l == *r) {
		l++;
		r++;
	}
	return *(const unsigned char *)l - *(const unsigned char *)r;
}

#ifdef TESTSUITE
TESTSUITE(strcmp) {
	static const char upper[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static const char lower[] = "abcdefghijklmnopqrstuvwxyz";
	CHECK(0 == strcmp("", ""));
	CHECK(0 == strcmp("a", "a"));
	CHECK(0 == strcmp("abc", "abc"));
	CHECK(0 > strcmp("abc", "abcd"));
	CHECK(0 < strcmp("abcd", "abc"));
	CHECK(0 > strcmp("abcd", "abce"));
	CHECK(0 < strcmp("abce", "abcd"));
	CHECK(0 < strcmp("a\x42", "a"));
	CHECK(0 < strcmp("a\x42", "a\x02"));
	CHECK(0 == strcmp(upper, upper));
	CHECK(0 < strcmp(lower, upper));
	CHECK(0 > strcmp(upper, lower));
	CHECK(0 > strcmp("foo", "foobar"));
	CHECK(0 < strcmp("foo", "fobbed"));
}
#endif


