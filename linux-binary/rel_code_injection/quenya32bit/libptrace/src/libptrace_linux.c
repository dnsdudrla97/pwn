/* libptrace, a process tracing and manipulation library.
 *
 * Dedicated to Merle Planten.
 *
 * Copyright (C) 2006-2008 Ronald Huizer
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include <config.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <asm/unistd.h>
#include <libptrace.h>
#include <libptrace_wrapper.h>

static int syscall_trap = SIGTRAP;

int ptrace_open(struct ptrace_context *p, ptrace_pid_t tid)
{
	return ptrace_attach(p, tid);
}

/* Open a process / thread given its identifier */
int ptrace_attach(struct ptrace_context *p, ptrace_pid_t tid)
{
	/* Initialize the error context first of all, as we might use
	 * it in this function.
	 */
	p->error.errmsg = NULL;
	PTRACE_ERR_CLEAR(p);

	if ( ptrace(PTRACE_ATTACH, tid, NULL, NULL) == -1 )
		goto out;

	do {
		int status;

		/* Wait for the child to STOP */
		if ( waitpid_no_EINTR(tid, &status, 0) == -1 )
			goto out_detach;

		/* If the child hasn't been stopped, it can only have exited
		 * here, so we jump to out.
		 */
		if ( !WIFSTOPPED(status) )
			goto out;

		/* If we have been stopped by a SIGSTOP, we can assume we
		 * caused it ourselves, and are done.
		 */
		if ( WSTOPSIG(status) == SIGSTOP )
			break;

		/* If not, we continue the process until we find it has been stopped
		 * by a SIGSTOP.
		 */
		if ( ptrace(PTRACE_CONT, tid, 0, WSTOPSIG(status)) == -1 )
			goto out_detach;
	} while (1);

#ifdef PTRACE_O_TRACESYSGOOD
	/* ptrace singlestep/syscall etc. will now cause the child to signal
	 * SIGTRAP orred with 0x80, to distinguish from real SIGTRAPs.
	 */
	if ( ptrace(PTRACE_SETOPTIONS, pid, NULL, PTRACE_O_TRACESYSGOOD) == 0 )
		syscall_trap |= 0x80;
#endif

	/* Initialize the ptrace library context */
	p->tid = tid;

	/* Initialize the current altstack as the original stack. */
	p->stack.flags = PTRACE_ALTSTACK_ORIG;

	return 0;

out_detach:
	PTRACE_ERR_SET_EXTERNAL(p);
	ptrace(PTRACE_DETACH, tid, NULL, NULL);
	if (0)	/* XXX: for now, avoid erroneous detach clubbering error */
out:
	PTRACE_ERR_SET_EXTERNAL(p);
	return -1;
}

int ptrace_close(struct ptrace_context *p)
{
	return ptrace_detach(p);
}

/* Close the traced process given its context */
int ptrace_detach(struct ptrace_context *p)
{
	if ( ptrace(PTRACE_DETACH, p->tid, NULL, NULL) == -1 ) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return -1;
	}

	if (p->error.errmsg != NULL)
		free(p->error.errmsg);

	PTRACE_ERR_CLEAR(p);

	return 0;
}

/* Write 'len' bytes from 'src' to the location 'dest' in the process described
 * in the ptrace_context 'p'.
 *
 * XXX: API might not conform to Windows stuff. If we have an error half-way,
 * then we'll have written stuff already. Does WriteProcessMemory test VM area
 * first?
 *
 * XXX: align reads/writes? Not too useful maybe, ptrace syscall overhead much
 * worse..
 */
int ptrace_write(struct ptrace_context *p, void *dest, const void *src, size_t len)
{
	size_t rem = len % sizeof(void *);
	size_t quot = len / sizeof(void *);
	unsigned char *s = (unsigned char *) src;
	unsigned char *d = (unsigned char *) dest;

	assert(sizeof(void *) == sizeof(long));

	while (quot-- != 0) {
		if ( ptrace(PTRACE_POKEDATA, p->tid, d, *(void **)s) == -1 )
			goto out_error;
		s += sizeof(void *);
		d += sizeof(void *);
	}

	/* We handle the last unpadded value here.
	 *
	 * Suppose we have the situation where we have written the string
	 * "ABCD" to 'dest', still want to write to the byte at *, but have an
	 * unadressable page at X. We'll find the ptrace write at 'X' returns
	 * an error, and will need to start writing at 'B' to satisfy this
	 * request.
	 *
	 * +---+---+---+---+---+---+
	 * | A | B | C | D | * | X |
	 * +---+---+---+---+---+---+
	 *
	 * This situation is handled in the code below, which is why it might
	 * look confusing.
	 */
	if (rem != 0) {
		long w;
		unsigned char *wp = (unsigned char *)&w;

		w = ptrace(PTRACE_PEEKDATA, p->tid, d, NULL);
		if (w == -1 && errno != 0) {
			d -= sizeof(void *) - rem;

			w = ptrace(PTRACE_PEEKDATA, p->tid, d, NULL);
			if (w == -1 && errno != 0)
				goto out_error;

			wp += sizeof(void *) - rem;
		}

		while (rem-- != 0)
			wp[rem] = s[rem];

		if (ptrace(PTRACE_POKEDATA, p->tid, d, w) == -1)
			goto out_error;
	}

	PTRACE_ERR_CLEAR(p);
	return 0;

out_error:
	PTRACE_ERR_SET_EXTERNAL(p);
	return -1;
}

/* Read 'len' bytes from 'src' in the process
 * described in the ptrace_context 'p' to the location 'dest'.
 */
int ptrace_read(struct ptrace_context *p, void *dest, const void *src, size_t len)
{
	long w;
	size_t rem = len % sizeof(void *);
	size_t quot = len / sizeof(void *);
	unsigned char *s = (unsigned char *) src;
	unsigned char *d = (unsigned char *) dest;

	assert(sizeof(void *) == sizeof(long));

	while (quot-- != 0) {
		w = ptrace(PTRACE_PEEKDATA, p->tid, s, NULL);
		if (w == -1 && errno != 0)
			goto out_error;
		*((long *)d) = w;

		s += sizeof(long);
		d += sizeof(long);
	}

	/* The remainder of data to read will be handled in a manner
	 * analogous to ptrace_write().
	 */
	if (rem != 0) {
		long w;
		unsigned char *wp = (unsigned char *)&w;

		w = ptrace(PTRACE_PEEKDATA, p->tid, s, NULL);
		if (w == -1 && errno != 0) {
			s -= sizeof(long) - rem;

			w = ptrace(PTRACE_PEEKDATA, p->tid, s, NULL);
			if (w == -1 && errno != 0)
				goto out_error;

			wp += sizeof(void *) - rem;
		}

		while (rem-- != 0)
			d[rem] = wp[rem];
	}

	PTRACE_ERR_CLEAR(p);
	return 0;

out_error:
	PTRACE_ERR_SET_EXTERNAL(p);
	return -1;
}

/* Generic register access functions
 */
int ptrace_get_registers(
	struct ptrace_context *p, struct ptrace_registers *regs
) {
	if ( ptrace(PTRACE_GETREGS, p->tid, NULL, regs) == -1 ) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return -1;
	}

	PTRACE_ERR_CLEAR(p);
	return 0;
}

int ptrace_set_registers(
	struct ptrace_context *p, struct ptrace_registers *regs
) {
	if ( ptrace(PTRACE_SETREGS, p->tid, NULL, regs) == -1 ) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return -1;
	}

	PTRACE_ERR_CLEAR(p);
	return 0;
}

const char *ptrace_errmsg(struct ptrace_context *p)
{
	if (p->error.errmsg != NULL) {
		free(p->error.errmsg);
		p->error.errmsg = NULL;
	}

	if (p->error.flags & PTRACE_ERR_FLAG_EXTERNAL)
		p->error.errmsg = strdup( strerror(p->error.external) );
	
	return p->error.errmsg;
}

int ptrace_continue(struct ptrace_context *pctx)
{
	if ( ptrace(PTRACE_CONT, pctx->tid, NULL, NULL) == -1 ) {
		PTRACE_ERR_SET_EXTERNAL(pctx);
		return -1;
	}

	return 0;
}

int ptrace_continue_signal(struct ptrace_context *pctx, int signum)
{
	unsigned long __signum = (unsigned long)signum;

	if ( ptrace(PTRACE_CONT, pctx->tid, NULL, (void *)__signum) == -1 ) {
		PTRACE_ERR_SET_EXTERNAL(pctx);
		return -1;
	}

	return 0;
}

int ptrace_wait_breakpoint(struct ptrace_context *pctx)
{
	return ptrace_wait_signal(pctx, SIGTRAP);
}

/* Continue running the remote process until we receive a signal of signum.
 */
int ptrace_wait_signal(struct ptrace_context *pctx, int signum)
{
	int status;

	if ( ptrace_continue(pctx) == -1 )
		return -1;

	do {
		if ( waitpid_no_EINTR(pctx->tid, &status, 0) == -1 ) {
			PTRACE_ERR_SET_EXTERNAL(pctx);
			return -1;
		}

		/* Child terminated normally */
		if ( WIFEXITED(status) ) {
			PTRACE_ERR_SET_INTERNAL(pctx, PTRACE_ERR_EXITED);
			return -1;
		}

		/* Child was terminated by a signal */
		if ( WIFSIGNALED(status) ) {
			PTRACE_ERR_SET_INTERNAL(pctx, PTRACE_ERR_EXITED);
			return -1;
		}

		/* The child was stopped by a signal; this is what we
		 * expected.  If it is not the signal we're looking for,
		 * delegate it to the child and continue.
		 */
		if ( WIFSTOPPED(status) && WSTOPSIG(status) != signum ) {
			if (ptrace_continue_signal(pctx, WSTOPSIG(status)) == -1)
				return -1;
		}
	} while (!WIFSTOPPED(status) || WSTOPSIG(status) != signum);

	return 0;
}

void *ptrace_malloc(struct ptrace_context *p, size_t size)
{
	void *ret;

	if (size == 0)
		size = 1;

	ret = ptrace_mmap(p, (void *) 0, size + sizeof(size_t),
	                  PROT_READ | PROT_WRITE | PROT_EXEC,
			  MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);

	if (ret == MAP_FAILED)
		return NULL;

	if (ptrace_write(p, ret, &size, sizeof(size_t)) == -1) {
		/* XXX: double fault */
		ptrace_munmap(p, ret, size + sizeof(size_t));
		return NULL;
	}

	return ret + sizeof(size_t);
}

int ptrace_free(struct ptrace_context *p, void *ptr)
{
	size_t size;

	if (ptr == NULL)
		return 0;
	
	ptr -= sizeof(size_t);
	if ( ptrace_read(p, &size, ptr, sizeof(size_t)) == -1 )
		return -1;

	return ptrace_munmap(p, ptr, size + sizeof(size_t));
}

int ptrace_get_pagesize(struct ptrace_context *p, int *page_size)
{
	long __page_size;

	errno = 0;
	__page_size = sysconf(_SC_PAGESIZE);
	if (__page_size == -1 && errno == EINVAL) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return -1;
	}

	*page_size = (int)__page_size;

	return 0;
}
