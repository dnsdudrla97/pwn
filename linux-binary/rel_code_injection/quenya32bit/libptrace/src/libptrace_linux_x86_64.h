/* libptrace, a process tracing and manipulation library.
 *
 * Dedicated to Kanna Ishihara.
 *
 * Copyright (C) 2006-2009 Ronald Huizer
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
#ifndef __LIBPTRACE_LINUX_X86_64_H
#define __LIBPTRACE_LINUX_X86_64_H

#include <stdint.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include "libptrace_config.h"

/* linux defines a software register called orig_rax; provide support for it.
 */
int ptrace_set_orig_rax(struct ptrace_context *p, uint64_t rax);
int ptrace_get_orig_rax(struct ptrace_context *p, uint64_t *rax);

/* Note that this structure has the same layout as struct
 * user_regs_struct under linux!
 * Libptrace depends on this, to keep the code cleaner, so do not change
 * this.
 */
struct ptrace_registers {
	unsigned long r15;
	unsigned long r14;
	unsigned long r13;
	unsigned long r12;
	unsigned long rbp;
	unsigned long rbx;
	unsigned long r11;
	unsigned long r10;
	unsigned long r9;
	unsigned long r8;
	unsigned long rax;
	unsigned long rcx;
	unsigned long rdx;
	unsigned long rsi;
	unsigned long rdi;
	unsigned long orig_rax;
	unsigned long rip;
	unsigned long cs;
	unsigned long rflags;
	unsigned long rsp;
	unsigned long ss;
	unsigned long fs_base;
	unsigned long gs_base;
	unsigned long ds;
	unsigned long es;
	unsigned long fs;
	unsigned long gs;
};

#endif	/* !__LIBPTRACE_LINUX_X86_64_H */
