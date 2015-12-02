// Copyright (C) 2015 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include <sys/entry.h>
#include <sys/stdio.h>
#include <stdio.h>
#include "stdio/stream.h"

extern int main(int argc, char *argv[]);

struct _stream _stdin;
FILE *stdin = &_stdin;
struct _stream _stdout;
FILE *stdout = &_stdout;
struct _stream _stderr;
FILE *stderr = &_stderr;

int _main(const char *cmdline)
{
	_init_stream(&_stdin);
	_init_stream(&_stdout);
	_init_stream(&_stderr);
	_stdin.id = _stdin_id;
	_stdout.id = _stdout_id;
	_stderr.id = _stderr_id;
	int ret = main(0, 0);
	fflush(0);
	return ret;
}

