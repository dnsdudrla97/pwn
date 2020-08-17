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
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <unistd.h>
#include "libptrace.h"

#define STACK_SIZE	(16384U)
#define CONTEXT_ALL \
	(CONTEXT_FULL | CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS)

static FARPROC kernel32_get_addr(struct ptrace_context *p, const char *sym)
{
	HANDLE k32, ret;

	k32 = GetModuleHandle("kernel32.dll");
	if (k32 == NULL) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return NULL;
	}
	
	ret = GetProcAddress(k32, sym);
	if (ret == NULL) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return NULL;
	}

	PTRACE_ERR_CLEAR(p);
	return ret;
}

HMODULE
ptrace_load_library(struct ptrace_context *p, const char *dll)
{
	void *dllname;
	FARPROC loadlib;
	HMODULE retval = NULL;
	struct ptrace_altstack stack, old_stack;

	/* Retrieve the address of LoadLibraryA for the current process. 
	 * As Windows maps in kernel32.dll at the same address for every
	 * process, we know this address is the same for the remote process.
	 */
	if ( (loadlib = kernel32_get_addr(p, "LoadLibraryA")) == NULL)
		goto out;

	/* We create and use our own custom stack in the target process and
	 * call LoadLibraryA(). This is to avoid ruining the data on stack past
	 * the top of the stack pointer, which could potentially be in use.
	 */
	if ( ptrace_altstack_init(p, &stack, STACK_SIZE) == -1 )
		goto out;

	if ( ptrace_altstack_switch(p, &stack, &old_stack) == -1 )
		goto out_altstack_destroy;

	/* Reserve space for the dll pathname in the remote process. */
	dllname = ptrace_malloc(p, strlen(dll) + 1);
	if (dllname == NULL)
		goto out_altstack_switch;

	/* Write out the dll name used for calling LoadLibraryA */
	if ( ptrace_write(p, dllname, dll, strlen(dll) + 1) == -1 )
		goto out_dllname_free;

	if ( ptrace_push32(p, (uint32_t) dllname) == -1 )
		goto out_dllname_free;

	/* Call the LoadLibraryA() function */
	ptrace_call_function(p, loadlib, (long *) &retval);

	/* XXX: handle double faults... */
out_dllname_free:
	if ( ptrace_free(p, dllname) == -1 )
		return NULL;

out_altstack_switch:
	if ( ptrace_altstack_switch(p, &old_stack, NULL) == -1 )
		return NULL;

out_altstack_destroy:
	if ( ptrace_altstack_destroy(p, &stack) == -1 )
		return NULL;
out:
	return retval;
}

BOOL
ptrace_free_library(struct ptrace_context *p, HMODULE dll)
{
	FARPROC freelib;
	BOOL retval = 0;
	struct ptrace_altstack stack, old_stack;

	/* Retrieve the address of FreeLibrary for the current process. 
	 * As Windows maps in kernel32.dll at the same address for every
	 * process, we know this address is the same for the remote process.
	 */
	if ( (freelib = kernel32_get_addr(p, "FreeLibrary")) == NULL)
		goto out;

	/* We create and use our own custom stack in the target process and
	 * call FreeLibrary(). This is to avoid ruining the data on stack past
	 * the top of the stack pointer, which could potentially be in use.
	 */
	if ( ptrace_altstack_init(p, &stack, STACK_SIZE) == -1 )
		goto out;

	if ( ptrace_altstack_switch(p, &stack, &old_stack) == -1 )
		goto out_altstack_destroy;

	if ( ptrace_push32(p, (uint32_t) dll) == -1 )
		goto out_altstack_switch;

	/* Call the FreeLibrary() function */
	ptrace_call_function(p, freelib, (long *) &retval);

	/* XXX: handle double faults... */
out_altstack_switch:
	if ( ptrace_altstack_switch(p, &old_stack, NULL) == -1 )
		return 0;

out_altstack_destroy:
	if ( ptrace_altstack_destroy(p, &stack) == -1 )
		return 0;
out:
	return retval;
}


FARPROC
ptrace_get_proc_address(struct ptrace_context *p, HMODULE dll, LPCSTR func)
{
	void *funcname;
	FARPROC getproc;
	FARPROC retval = NULL;
	struct ptrace_altstack stack, old_stack;

	/* Retrieve the address of GetProcAddress for the current process. 
	 * As Windows maps in kernel32.dll at the same address for every
	 * process, we know this address is the same for the remote process.
	 */
	if ( (getproc = kernel32_get_addr(p, "GetProcAddress")) == NULL)
		goto out;

	/* We create and use our own custom stack in the target process and
	 * call LoadLibraryA(). This is to avoid ruining the data on stack past
	 * the top of the stack pointer, which could potentially be in use.
	 */
	if ( ptrace_altstack_init(p, &stack, STACK_SIZE) == -1 )
		goto out;

	if ( ptrace_altstack_switch(p, &stack, &old_stack) == -1 )
		goto out_altstack_destroy;

	/* Reserve space for the dll pathname in the remote process. */
	funcname = ptrace_malloc(p, strlen(func) + 1);
	if (funcname == NULL)
		goto out_altstack_switch;

	/* Write out the dll name used for calling GetProcAddress */
	if ( ptrace_write(p, funcname, func, strlen(func) + 1) == -1 )
		goto out_funcname_free;

	if ( ptrace_push32(p, (uint32_t) funcname) == -1 )
		goto out_funcname_free;

	if ( ptrace_push32(p, (uint32_t) dll) == -1 )
		goto out_funcname_free;

	/* Call the GetProcAddress() function */
	ptrace_call_function(p, getproc, (long *) &retval);

	/* XXX: handle double faults... */
out_funcname_free:
	if ( ptrace_free(p, funcname) == -1 )
		return NULL;

out_altstack_switch:
	if ( ptrace_altstack_switch(p, &old_stack, NULL) == -1 )
		return NULL;

out_altstack_destroy:
	if ( ptrace_altstack_destroy(p, &stack) == -1 )
		return NULL;
out:
	return retval;
}

/* XXX: later we can optimize by not requesting CONTEXT_ALL for functions
 * accessing the individual registers.
 */
int
ptrace_get_registers(struct ptrace_context *p, struct ptrace_registers *r)
{
	r->ContextFlags = CONTEXT_FULL;
	if ( GetThreadContext(p->thandle, (CONTEXT *) r) == 0) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return -1;
	} else {
		PTRACE_ERR_CLEAR(p);
		return 0;
	}
}

int
ptrace_set_registers(struct ptrace_context *p, struct ptrace_registers *r)
{
	r->ContextFlags = CONTEXT_FULL;
	if ( SetThreadContext(p->thandle, (CONTEXT *) r) == 0 ) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return -1;
	} else {
		PTRACE_ERR_CLEAR(p);
		return 0;
	}
}

int ptrace_save_fpu_state(struct ptrace_context *p,
                          struct ptrace_fpu_state *fp)
{
	CONTEXT *ctx = (CONTEXT *) fp;

	ctx->ContextFlags = CONTEXT_FLOATING_POINT |
	                    CONTEXT_EXTENDED_REGISTERS;

	if ( GetThreadContext(p->thandle, ctx) == 0 ) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return -1;
	}

	PTRACE_ERR_CLEAR(p);
	return 0;
}

int ptrace_load_fpu_state(struct ptrace_context *p,
                          struct ptrace_fpu_state *fp)
{
	CONTEXT *ctx = (CONTEXT *) fp;

	ctx->ContextFlags = CONTEXT_FLOATING_POINT |
	                    CONTEXT_EXTENDED_REGISTERS;

	if ( SetThreadContext(p->thandle, ctx) == 0 ) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return -1;
	}

	PTRACE_ERR_CLEAR(p);
	return 0;
}

/* Register access functions
 */
#define DEFINE_PTRACE_GET_REG(r)					\
int ptrace_get_##r(struct ptrace_context *pctx, uint32_t *r)		\
{									\
	struct ptrace_registers regs;					\
									\
	if ( ptrace_get_registers(pctx, &regs) == -1 )			\
		return -1;						\
									\
	*r = regs.r;							\
									\
	return 0;							\
}

#define DEFINE_PTRACE_GET_SEG_REG(r)					\
int ptrace_get_##r(struct ptrace_context *pctx, uint16_t *r)		\
{									\
	struct ptrace_registers regs;					\
									\
	if (ptrace_get_registers(pctx, &regs) == -1)			\
		return -1;						\
									\
	*r = regs.r;							\
									\
	return 0;							\
}

#define DEFINE_PTRACE_SET_REG(r)					\
int ptrace_set_##r(struct ptrace_context *pctx, uint32_t r)		\
{									\
	struct ptrace_registers regs;					\
									\
	if ( ptrace_get_registers(pctx, &regs) == -1 )			\
		return -1;						\
									\
	regs.r = r;							\
									\
	if ( ptrace_set_registers(pctx, &regs) == -1 )			\
		return -1;						\
									\
	return 0;							\
}

#define DEFINE_PTRACE_SET_SEG_REG(r)					\
int ptrace_set_##r(struct ptrace_context *pctx, uint16_t r)		\
{									\
	struct ptrace_registers regs;					\
									\
	if (ptrace_get_registers(pctx, &regs) == -1)			\
		return -1;						\
									\
	regs.r = r;							\
									\
	if (ptrace_set_registers(pctx, &regs) == -1)			\
		return -1;						\
									\
	return 0;							\
}


DEFINE_PTRACE_GET_REG(eax)
DEFINE_PTRACE_GET_REG(ebx)
DEFINE_PTRACE_GET_REG(ecx)
DEFINE_PTRACE_GET_REG(edx)
DEFINE_PTRACE_GET_REG(esi)
DEFINE_PTRACE_GET_REG(edi)
DEFINE_PTRACE_GET_REG(esp)
DEFINE_PTRACE_GET_REG(ebp)
DEFINE_PTRACE_GET_REG(eip)
DEFINE_PTRACE_GET_REG(eflags)
DEFINE_PTRACE_GET_SEG_REG(cs)
DEFINE_PTRACE_GET_SEG_REG(ds)
DEFINE_PTRACE_GET_SEG_REG(es)
DEFINE_PTRACE_GET_SEG_REG(fs)
DEFINE_PTRACE_GET_SEG_REG(gs)
DEFINE_PTRACE_GET_SEG_REG(ss)

DEFINE_PTRACE_SET_REG(eax)
DEFINE_PTRACE_SET_REG(ebx)
DEFINE_PTRACE_SET_REG(ecx)
DEFINE_PTRACE_SET_REG(edx)
DEFINE_PTRACE_SET_REG(esi)
DEFINE_PTRACE_SET_REG(edi)
DEFINE_PTRACE_SET_REG(esp)
DEFINE_PTRACE_SET_REG(ebp)
DEFINE_PTRACE_SET_REG(eip)
DEFINE_PTRACE_SET_REG(eflags)
DEFINE_PTRACE_SET_SEG_REG(cs)
DEFINE_PTRACE_SET_SEG_REG(ds)
DEFINE_PTRACE_SET_SEG_REG(es)
DEFINE_PTRACE_SET_SEG_REG(fs)
DEFINE_PTRACE_SET_SEG_REG(gs)
DEFINE_PTRACE_SET_SEG_REG(ss)
