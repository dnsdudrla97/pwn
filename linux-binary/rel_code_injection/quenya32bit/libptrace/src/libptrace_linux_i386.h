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
#ifndef __LIBPTRACE_LINUX_I386_H
#define __LIBPTRACE_LINUX_I386_H

#include <stdint.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include "libptrace_config.h"

#define ELF_HEADER_BASE ((void *)0x8048000)

struct ptrace_cpu_state {
	uint8_t __buf[sizeof(struct user_regs_struct)];
};

/* linux defines a software register called orig_eax; provide support for it.
 */
int ptrace_set_orig_eax(struct ptrace_context *p, uint32_t eax);
int ptrace_get_orig_eax(struct ptrace_context *p, uint32_t *eax);

/* Note that this structure has the same layout as struct
 * user_regs_struct under linux!
 * Libptrace depends on this, to keep the code cleaner, so do not change
 * this.
 */
struct ptrace_registers {
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
	uint32_t esi;
	uint32_t edi;
	uint32_t ebp;
	uint32_t eax;
	uint32_t ds;
	uint32_t es;
	uint32_t fs;
	uint32_t gs;
	uint32_t orig_eax;
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t esp;
	uint32_t ss;
};

#endif	/* !__LIBPTRACE_LINUX_I386_H */
