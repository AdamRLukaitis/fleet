// Copyright (C) 2015 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef _TIME_H
#define _TIME_H

#include <c/null.h>
#include <c/size_t.h>
#include <c/struct_tm.h>

typedef int clock_t;
typedef int time_t;
// millisecond clock
#define CLOCKS_PER_SEC 1000

clock_t clock(void);
double difftime(time_t end, time_t start);
time_t mktime(struct tm*);
time_t time(time_t *out);
char *asctime(const struct tm*);
char *ctime(const time_t*);
struct tm *gmtime(const time_t*);
struct tm *localtime(const time_t*);
size_t strftime(char *buf, size_t size, const char *format, const struct tm*);

#endif //_TIME_H

