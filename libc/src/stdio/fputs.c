// Copyright (C) 2015 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include <string.h>
#include <stdio.h>

int fputs(const char *str, FILE *stream)
{
	size_t bytes = strlen(str);
	return fwrite(str, sizeof(char), bytes, stream) == bytes? bytes: EOF;
}

