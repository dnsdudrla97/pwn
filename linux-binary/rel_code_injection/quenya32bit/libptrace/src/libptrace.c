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
#include <stdio.h>
#include <libptrace.h>

/** Read a C-style string from a remote process.
 *
 * Retrieves a 0-terminated C-style string from a remote process into a local
 * array, possibly truncating the string if the array is too small.  The
 * result will always be 0-terminated.
 *
 * \param pctx Pointer to the ptrace_context of the traced thread.
 * \param dst Pointer to the buffer which the string will be read to.
 * \param size Size of the buffer the string is read to.
 * \param src Pointer to the location of the string in the remote process.
 *
 * \return 0 on success, -1 on failure.
 */
int
ptrace_read_string(struct ptrace_context *pctx, char *dst, size_t size,
                   const char *src)
{
	size_t i;
	char byte = 42;

	if (size == 0)
		return 0;

	for (i = 0; i < size && byte != 0; i++) {
		if ( ptrace_read(pctx, &byte, src + i, 1) == -1 )
			return -1;
		dst[i] = byte;
	}

	/* 0-terminate the string properly. */
	dst[i - 1] = 0;

	return 0;
}

/** Retrieve the length of a C-style string from a remote process.
 *
 * Retrieves the length of a 0-terminated C-style string from a remote
 * process.
 *
 * \param pctx Pointer to the ptrace_context of the traced thread.
 * \param string Pointer to the location of the string in the remote process.
 * \param size Pointer to the size_t which will hold the string length.
 *
 * \return 0 on success, -1 on failure.
 */
int
ptrace_strlen(struct ptrace_context *pctx, const char *string, size_t *size)
{
	char byte;
	size_t length = 0;

	do {
		if ( ptrace_read(pctx, &byte, string, 1) == -1 )
			return -1;
		length++;
	} while (byte != 0);

	*size = length - 1;
	return 0;
}
