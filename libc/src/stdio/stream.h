#ifndef _LIBC_INTERNAL_STREAM_H
#define _LIBC_INTERNAL_STREAM_H

#include <stdio.h>

#define STREAM_EOF 0x01 // C standard feof() indicator
#define STREAM_ERR 0x02 // C standard ferror() indicator
#define STREAM_ALLOC 0x04 // we own the buffer and must free() it on close
#define STREAM_LINESYNC 0x08 // must flush buffer after each newline
#define STREAM_READ 0x0F // currently in read mode
#define STREAM_WRITE 0x10 // currently in write mode

struct _stream
{
	// list of active streams, so we can flush all / close all when necessary
	struct _stream *next;
	struct _stream **back;
	// bitmask with some combination of above flags
	unsigned state;
	// stream id used in kernel calls
	int id;
	// buffer location & size, if present
	char *buf_addr;
	size_t buf_size;
	// number of data bytes contained in the buffer
	size_t buf_count;
};

extern struct _stream *_stream_list;
void _init_stream(struct _stream*);
void _exit_stream(struct _stream*);

#endif //_LIBC_INTERNAL_STREAM_H