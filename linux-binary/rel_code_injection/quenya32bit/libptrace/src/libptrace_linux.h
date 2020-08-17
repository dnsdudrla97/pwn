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
#ifndef __LIBPTRACE_LINUX_H
#define __LIBPTRACE_LINUX_H

#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "libptrace_x86.h"

/* XXX: fugly. */
typedef pid_t	ptrace_pid_t;
typedef void *	ptrace_library_handle_t;
typedef void *	ptrace_function_ptr_t;

//#include "libptrace_elf.h"

#ifdef __i386__
  #include "libptrace_linux_i386.h"
#endif

#ifdef __x86_64__
  #include "libptrace_linux_x86_64.h"
#endif

#define PTRACE_ERR_CLEAR(p)						\
	do {								\
		p->error.external = 0;					\
		p->error.internal = PTRACE_ERR_NONE;			\
		p->error.flags = PTRACE_ERR_FLAG_NONE;			\
	} while(0)

#define PTRACE_ERR_SET_EXTERNAL(p)					\
	do {								\
		p->error.external = errno;				\
		p->error.internal = PTRACE_ERR_NONE;			\
		p->error.flags = PTRACE_ERR_FLAG_EXTERNAL;		\
	} while(0)

#define PTRACE_ERR_SET_INTERNAL(p, e)					\
	do {								\
		p->error.external = 0;					\
		p->error.internal = e;					\
		p->error.flags = PTRACE_ERR_FLAG_NONE;			\
	} while(0)

/* We do not need to distinguish between external and internal errors for
 * remote errors, as there is no such thing as a remote libptrace internal
 * error.
 */
#define PTRACE_ERR_SET_REMOTE(p, e)					\
	do {								\
		p->error.external = e;					\
		p->error.internal = PTRACE_ERR_NONE;			\
		p->error.flags = PTRACE_ERR_FLAG_REMOTE |		\
		                 PTRACE_ERR_FLAG_EXTERNAL;		\
	} while(0)


struct ptrace_error
{
	int	internal:24;
	int	flags:8;

	int	external;
	char	*errmsg;
};

struct ptrace_context
{
	int state;
	ptrace_pid_t tid;
	struct ptrace_error error;
	struct ptrace_altstack stack;
};

/* Linux specific functions. */
int ptrace_get_pagesize(struct ptrace_context *p, int *page_size);

int ptrace_wait_signal(struct ptrace_context *pctx, int signum);

void *ptrace_mmap(struct ptrace_context *pctx, void *start, size_t length,
                  int prot, int flags, int fd, off_t offset);
int ptrace_munmap(struct ptrace_context *pctx, void *start, size_t length);

#endif
