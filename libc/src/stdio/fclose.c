#include "internal/bufio.h"
#include "internal/stream.h"

int fclose(FILE *stream)
{
	int syncret = _sync(stream);
	int closeret = _close(stream);
	return (syncret || closeret)? EOF: 0;
}

