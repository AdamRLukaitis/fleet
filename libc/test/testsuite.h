// Copyright (C) 2015 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef __TESTSUITE_H
#define __TESTSUITE_H

#ifdef TESTSUITE
#undef TESTSUITE
#else
#error testsuite code must be guarded by #ifdef TESTSUITE
#endif

#define TESTSUITE(x) __attribute__((constructor)) static void _test_##x()

void testlogf(const char *fmt, ...);

void check(int expect, const char *cond, const char *func, int line);
#define CHECK(cond) \
		check((cond), #cond, __FILE__, __LINE__)

void check_mem(
		const void *actual,
		const void *expect,
		int bytes,
		const char *func,
		int line);
#define CHECK_MEM(actual, expect, bytes) \
		check_mem((actual), (expect), (bytes), __FILE__, __LINE__)

void check_str(
		const char *actual,
		const char *expect,
		int max,
		const char *func,
		int line);
#define CHECK_STR(actual, expect, max) \
		check_str((actual), (expect), (max), __FILE__, __LINE__)


// libc expects to invoke system calls defined in libkernel.
// when running in the test environment, these calls would get resolved
// against equivalently-named functions in the host system's libc, which
// would not really work the way anyone hoped. We will rename these functions.
#define open libc_test_open
#define read libc_test_read
#define write libc_test_write
#define close libc_test_close

#endif //__TESTSUITE_H

