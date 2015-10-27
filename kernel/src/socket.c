#include <sys/socket.h>
#include <sys/errno.h>

struct socket {
	void *ref;
	struct iops *ops;
};

#define OPEN_MAX 256
static struct socket sockets[OPEN_MAX];

int open(void *ref, struct iops *ops)
{
	for (int i = 0; i < OPEN_MAX; ++i) {
		struct socket *s = &sockets[i];
		if (0 == s->ops) {
			s->ref = ref;
			s->ops = ops;
			return i;
		}
	}
	return -EMFILE;
}

int write(int id, const void *src, unsigned bytes)
{
	if (id < 0 || id >= OPEN_MAX) return -EBADF;
	struct socket *s = &sockets[id];
	if (!s->ops) return -EBADF;
	if (!s->ops->write) return -EPERM;
	return s->ops->write(s->ref, src, bytes);
}

int read(int id, void *dest, unsigned bytes)
{
	if (id < 0 || id >= OPEN_MAX) return -EBADF;
	struct socket *s = &sockets[id];
	if (!s->ops) return -EBADF;
	if (!s->ops->read) return -EPERM;
	return s->ops->read(s->ref, dest, bytes);
}

int close(int id)
{
	if (id < 0 || id >= OPEN_MAX) return -EBADF;
	struct socket *s = &sockets[id];
	if (!s->ops) return -EBADF;
	if (s->ops->close) s->ops->close(s->ref);
	s->ops = 0;
	return 0;
}
