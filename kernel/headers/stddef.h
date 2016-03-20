// Copyright (C) 2015 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef _STDDEF_H
#define _STDDEF_H

#define NULL ((void*)0)
typedef unsigned size_t;
typedef __WCHAR_TYPE__ wchar_t;
typedef int ptrdiff_t;
#define offsetof(type, member) __builtin_offsetof(type, member)

#endif //_STDDEF_H

