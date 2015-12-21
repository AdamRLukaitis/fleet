// Copyright (C) 2015 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef _STDLIB_H
#define _STDLIB_H

#include <_null.h>
#include <_size_t.h>
#include <_wchar_t.h>

double atof(const char *str);
int atoi(const char *str);
long atol(const char *str);
long long atoll(const char *str);
float strtof(const char *str, char **end);
double strtod(const char *str, char **end);
long double strtold(const char *str, char **end);
long strtol(const char *str, char **end, int radix);
long long strtoll(const char *str, char **end, int radix);
unsigned long strtoul(const char *str, char **end, int radix);
unsigned long long strtoull(const char *str, char **end, int radix);

typedef int(*_compar)(const void*, const void*);
void *bsearch (const void *key, const void *base, size_t n, size_t w, _compar);
void qsort(void *base, size_t n, size_t w, _compar);

int abs(int);
long labs(long);
long long llabs(long long);

typedef struct { int quot, rem; } div_t;
typedef struct { long quot, rem; } ldiv_t;
typedef struct { long long quot, rem; } lldiv_t;

div_t div(int, int);
ldiv_t ldiv(long, long);
lldiv_t lldiv(long long, long long);

#define RAND_MAX (0x7FFFFFFF)
int rand(void);
void srand(unsigned seed);

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
void _Exit(int status);
int atexit(void (*func)(void));
void exit(int status);

void *malloc(size_t bytes);
void *calloc(size_t num, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

#if 0 // still to implement
MB_CUR_MAX
abort
getenv
mblen
mbstowcs
mbtowc
system
wcstombs
wctomb
#endif // still to implement

#endif //_STDLIB_H

