#include "internal/stdio.h"

size_t fwrite(const void *src, size_t size, size_t count, FILE *stream)
{
	_write(stream, src, size*count) / size;
}

