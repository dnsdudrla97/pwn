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
#ifndef __LIBPTRACE_WINDOWS_H
#define __LIBPTRACE_WINDOWS_H

#include <windows.h>
#ifdef __i386__
  #include "libptrace_windows_i386.h"
#endif

#ifndef SIGTRAP
  #define SIGTRAP	5
#endif

#define PTRACE_ERR_CLEAR(p)						\
	do {								\
		p->error.external = 0;					\
		p->error.internal = PTRACE_ERR_NONE;			\
		p->error.flags = PTRACE_ERR_FLAG_NONE;			\
	} while(0)

#define PTRACE_ERR_SET_EXTERNAL(p)					\
	do {								\
		p->error.external = GetLastError();			\
		p->error.internal = PTRACE_ERR_NONE;			\
		p->error.flags = PTRACE_ERR_FLAG_EXTERNAL;		\
	} while(0)

#define PTRACE_ERR_SET_INTERNAL(p, e)					\
	do {								\
		p->error.external = 0;					\
		p->error.internal = e;					\
		p->error.flags = PTRACE_ERR_FLAG_NONE;			\
	} while(0)

#define PTRACE_ERR_SET_REMOTE(p, e)					\
	do {								\
		p->error.external = e;					\
		p->error.internal = PTRACE_ERR_NONE;			\
		p->error.flags = PTRACE_ERR_FLAG_REMOTE |		\
		                 PTRACE_ERR_FLAG_EXTERNAL;		\
	} while(0)

typedef DWORD	ptrace_pid_t;
typedef HMODULE	ptrace_library_handle_t;
typedef FARPROC	ptrace_function_ptr_t;

struct ptrace_error
{
	int	internal:24;
	int	flags:8;
	void	*errmsg;

	DWORD	external;
};

struct ptrace_context
{
	HANDLE phandle;
	HANDLE thandle;
	ptrace_pid_t pid;
	ptrace_pid_t tid;
	struct ptrace_error error;
	struct ptrace_altstack stack;
	DWORD continue_status;
};

DWORD WINAPI GetProcessIdOfThread(HANDLE hThread);
HMODULE ptrace_load_library(struct ptrace_context *p, const char *dllname);
BOOL ptrace_free_library(struct ptrace_context *p, HMODULE dll);
FARPROC ptrace_get_proc_address(struct ptrace_context *p, HMODULE dll, LPCSTR func);

int ptrace_get_pagesize(struct ptrace_context *p, int *page_size);

#endif
