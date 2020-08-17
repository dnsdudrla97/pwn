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
#ifndef __LIBPTRACE_WINDOWS_I386_H
#define __LIBPTRACE_WINDOWS_I386_H

/* As copied from winnt.h, with registers renamed to the libptrace naming
 * convention.
 *
 * Do not change this layout without reason; the struct layout needs to be
 * the same as the layout of CONTEXT.
 */

struct ptrace_floating_save_area {
	DWORD   ControlWord;
	DWORD   StatusWord;
	DWORD   TagWord;
	DWORD   ErrorOffset;
	DWORD   ErrorSelector;
	DWORD   DataOffset;
	DWORD   DataSelector;
	BYTE    RegisterArea[80];
	DWORD   Cr0NpxState;
};

struct ptrace_registers {
	DWORD   ContextFlags;
	DWORD   Dr0;
	DWORD   Dr1;
	DWORD   Dr2;
	DWORD   Dr3;
	DWORD   Dr6;
	DWORD   Dr7;
	struct ptrace_floating_save_area FloatSave;
	DWORD   gs;
	DWORD   fs;
	DWORD   es;
	DWORD   ds;
	DWORD   edi;
	DWORD   esi;
	DWORD   ebx;
	DWORD   edx;
	DWORD   ecx;
	DWORD   eax;
	DWORD   ebp;
	DWORD   eip;
	DWORD   cs;
	DWORD   eflags;
	DWORD   esp;
	DWORD   ss;
	BYTE    ExtendedRegisters[MAXIMUM_SUPPORTED_EXTENSION];
};

struct ptrace_fpu_state {
	uint8_t __buf[sizeof(CONTEXT)];
};

#endif
