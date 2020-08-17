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
#ifndef __LIBPTRACE_H
#define __LIBPTRACE_H

#include <stdio.h>
#include <stdint.h>
#include <wchar.h>
#include "libptrace_error.h"

#if defined(__i386__) || defined(__x86_64__)
  #include "libptrace_x86.h"
#endif

#ifdef WIN32
  #include "libptrace_windows.h"
#endif
#ifdef __linux__
  #include "libptrace_linux.h"
#endif

int ptrace_open(struct ptrace_context *p, ptrace_pid_t tid);
int ptrace_attach(struct ptrace_context *p, ptrace_pid_t tid);

int ptrace_close(struct ptrace_context *p);
int ptrace_detach(struct ptrace_context *p);

int ptrace_write(struct ptrace_context *p, void *, const void *, size_t);
int ptrace_read(struct ptrace_context *p, void *, const void *, size_t);

int ptrace_read_string(struct ptrace_context *pctx, char *dst, size_t size,
                       const char *src);
int ptrace_strlen(struct ptrace_context *pctx, const char *string,
                  size_t *size);

int ptrace_get_registers(struct ptrace_context *, struct ptrace_registers *);
int ptrace_set_registers(struct ptrace_context *, struct ptrace_registers *);

int ptrace_save_cpu_state(struct ptrace_context *, struct ptrace_cpu_state *);
int ptrace_load_cpu_state(struct ptrace_context *, struct ptrace_cpu_state *);

void *ptrace_malloc(struct ptrace_context *p, size_t len);
int ptrace_free(struct ptrace_context *p, void *mem);

const char *ptrace_errmsg(struct ptrace_context *p);
const wchar_t *ptrace_errmsg16(struct ptrace_context *p);

int ptrace_wait_breakpoint(struct ptrace_context *p);
int ptrace_wait_breakpoint_at(struct ptrace_context *p, void *location);

ptrace_library_handle_t ptrace_library_load(struct ptrace_context *pctx,
                                            const char *libname);
ptrace_function_ptr_t
ptrace_library_get_function_addr(struct ptrace_context *pctx,
                                 ptrace_library_handle_t handle,
                                 const char *function);
int ptrace_library_unload(struct ptrace_context *pctx,
                          ptrace_library_handle_t handle);
#endif	/* !__LIBPTRACE_H */
