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
#ifndef __LIBPTRACE_LINUX_X86_H
#define __LIBPTRACE_LINUX_X86_H

#if defined(__i386__)
#include <libptrace_linux_i386.h>
#elif defined(__x86_64__)
#include <libptrace_linux_x86_64.h>
#endif

#endif
