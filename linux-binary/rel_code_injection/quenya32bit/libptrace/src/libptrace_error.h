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
#ifndef __LIBPTRACE_ERROR_H
#define __LIBPTRACE_ERROR_H

#include <limits.h>

/* libptrace internal error types
 */
#define PTRACE_ERR_NONE			0	/* No error */
#define PTRACE_ERR_EXITED		1	/* Remote process exited */
#define PTRACE_ERR_PAGESIZE		2	/* Unsuitable page size */
#define PTRACE_ERR_ALTSTACK_ORIG	3	/* Original stack error */
#define PTRACE_ERR_ALTSTACK_INUSE	4	/* Stack currently in use */

/* libtrace error flags
 *
 * PTRACE_ERR_FLAG_REMOTE	Specifies an error occured in the remote process.
 * PTRACE_ERR_FLAG_EXTERNAL	Specifies the error is not an libptrace internal error.
 */
#define PTRACE_ERR_FLAG_NONE		0
#define PTRACE_ERR_FLAG_REMOTE		1
#define PTRACE_ERR_FLAG_EXTERNAL	2

#endif
