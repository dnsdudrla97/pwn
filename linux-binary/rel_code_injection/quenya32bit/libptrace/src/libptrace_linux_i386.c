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
#include <config.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <asm/unistd.h>
#include "libptrace.h"
#include "libptrace_i386.h"

#define SYSCALL_ERROR(x) ((unsigned int) (x) >= (unsigned int)(-(128 + 1)))

int ptrace_wait_breakpoint_at(struct ptrace_context *pctx, void *location)
{
	uint32_t eip;

	do {
		if ( ptrace_wait_signal(pctx, SIGTRAP) == -1 )
			return -1;

		if ( ptrace_get_eip(pctx, &eip) == -1 )
			return -1;
	} while ( (uint32_t) location != eip);

	return 0;
}

/* Perform a mmap systemcall in the remote process.
 */
void *ptrace_mmap(struct ptrace_context *p, void *start, size_t length,
                  int prot, int flags, int fd, off_t offset)
{
	int retval;

	uint8_t __stub[] = {
		/* code area */
		0xe8, 0x18, 0x00, 0x00, 0x00,	/* call 18h	*/
		/* data area */
		0x00, 0x00, 0x00, 0x00,		/* start	*/
		0x00, 0x00, 0x00, 0x00,		/* length	*/
		0x00, 0x00, 0x00, 0x00,		/* prot		*/
		0x00, 0x00, 0x00, 0x00,		/* flags	*/
		0x00, 0x00, 0x00, 0x00,		/* fd		*/
		0x00, 0x00, 0x00, 0x00,		/* offset	*/
		/* code area */
		0x5b,				/* pop ebx	*/
		0xb8, 0x00, 0x00, 0x00, 0x00,	/* mov eax, 0	*/
		0xcd, 0x80			/* int 80h	*/
	};

	/* Fill in all dynamic data in our code stub */
	*(uint32_t *)(__stub + 5) = (uint32_t) start;
	*(uint32_t *)(__stub + 9) = (uint32_t) length;
	*(uint32_t *)(__stub + 13) = (uint32_t) prot;
	*(uint32_t *)(__stub + 17) = (uint32_t) flags;
	*(uint32_t *)(__stub + 21) = (uint32_t) fd;
	*(uint32_t *)(__stub + 25) = (uint32_t) offset;
	*(uint32_t *)(__stub + 31) = (uint32_t) __NR_mmap;

	if ( __ptrace_run_code(p, __stub, sizeof(__stub), &retval) == -1 )
		return MAP_FAILED;

	/* If the systemcall in the remote process produced an error, we
	 * set up a remote error in the ptrace context.
	 */
	if ( SYSCALL_ERROR(retval) ) {
		PTRACE_ERR_SET_REMOTE(p, -retval);
		return MAP_FAILED;
	}

	PTRACE_ERR_CLEAR(p);
	return (void *)retval;
}

int ptrace_munmap(struct ptrace_context *p, void *start, size_t length)
{
	int retval;
	uint8_t __stub[] = {
		0xb8, 0x00, 0x00, 0x00, 0x00,	/* mov eax, 0	*/
		0xbb, 0x00, 0x00, 0x00, 0x00,	/* mov ebx, 0	*/
		0xb9, 0x00, 0x00, 0x00, 0x00,	/* mov ecx, 0	*/
		0xcd, 0x80			/* int 80h	*/
	};

	/* Fill in all dynamic data in our code stub */
	*(uint32_t *)(__stub + 1) = (uint32_t) __NR_munmap;
	*(uint32_t *)(__stub + 6) = (uint32_t) start;
	*(uint32_t *)(__stub + 11) = (uint32_t) length;

	if ( __ptrace_run_code(p, __stub, sizeof(__stub), &retval) == -1 )
		return -1;

	if ( SYSCALL_ERROR(retval) ) {
		PTRACE_ERR_SET_REMOTE(p, -retval);
		return -1;
	}

	PTRACE_ERR_CLEAR(p);
	return 0;
}

/* State saving functions.
 */
int ptrace_save_cpu_state(
	struct ptrace_context *p,
	struct ptrace_cpu_state *st
) {
	if ( ptrace(PTRACE_GETREGS, p->tid, NULL, st) == -1 ) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return -1;
	}

	PTRACE_ERR_CLEAR(p);
	return 0;
}

int ptrace_load_cpu_state(
	struct ptrace_context *p,
	struct ptrace_cpu_state *st
) {
	if ( ptrace(PTRACE_SETREGS, p->tid, NULL, st) == -1 ) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return -1;
	}

	PTRACE_ERR_CLEAR(p);
	return 0;
}

int ptrace_save_fpu_state(
	struct ptrace_context *p,
	struct ptrace_fpu_state *st
) {
	if ( ptrace(LIBPTRACE_GETFPUSTATE, p->tid, NULL, st) == -1 ) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return -1;
	}

	PTRACE_ERR_CLEAR(p);
	return 0;
}

int ptrace_load_fpu_state(
	struct ptrace_context *p,
	struct ptrace_fpu_state *st
) {
	if ( ptrace(LIBPTRACE_SETFPUSTATE, p->tid, NULL, st) == -1 ) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return -1;
	}

	PTRACE_ERR_CLEAR(p);
	return 0;
}

/* Register access functions
 */
int ptrace_get_fpu_registers(
	struct ptrace_context *p,
	struct ptrace_fpu_registers *r
) {
	struct user_fpregs_struct __r;

	if ( ptrace(PTRACE_GETFPREGS, p->tid, NULL, &__r) == -1 ) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return -1;
	}

	/* We convert the floating point register data to the format libptrace
	 * uses for the i386 architecture.
	 * Linux tends to or in 0xFFFF words, and doesn't prepend 11011b to the
	 * opcodes, which we like.
	 */
	r->cwd = __r.cwd & 0xffff;
	r->swd = __r.swd & 0xffff;
	r->twd = __r.twd & 0xffff;
	r->fip = __r.fip;
	r->fcs = __r.fcs & 0xffff;
	r->foc = ((__r.fcs >> 16) & 0x07ff) | 0xd800;
	r->fop = __r.foo;
	r->fos = __r.fos & 0xffff;

	memcpy(r->st_space, __r.st_space, sizeof(__r.st_space));

	PTRACE_ERR_CLEAR(p);
	return 0;
}

int ptrace_set_fpu_registers(
	struct ptrace_context *p,
	struct ptrace_fpu_registers *r
) {
	struct user_fpregs_struct __r;

	/* XXX: we might want to error when r->foc has the five
	 * bits set to not 11011b...
	 */
	__r.cwd = r->cwd | 0xffff0000;
	__r.swd = r->swd | 0xffff0000;
	__r.twd = r->twd | 0xffff0000;
	__r.fip = r->fip;
	__r.fcs = ((r->foc & 0x7ff) << 16) | r->fcs;
	__r.foo = r->fop;
	__r.fos = r->fos;

	memcpy(__r.st_space, r->st_space, sizeof(__r.st_space));

	if ( ptrace(PTRACE_SETFPREGS, p->tid, NULL, &__r) == -1 ) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return -1;
	}

	PTRACE_ERR_CLEAR(p);
	return 0;
}

#ifdef USE_MMX
int ptrace_get_mmx_registers(
	struct ptrace_context *p,
	struct ptrace_mmx_registers *r
) {
	struct user_fpregs_struct __r;
	uint16_t *mmx_regp = (uint16_t *)__r.st_space;

	if ( ptrace(PTRACE_GETFPREGS, p->tid, NULL, &__r) == -1 ) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return -1;
	}

	/* We convert the floating point register data to the format libptrace
	 * uses for the i386 architecture.
	 * Linux tends to or in 0xFFFF words, and doesn't prepend 11011b to the
	 * opcodes, which we like.
	 */
	r->cwd = __r.cwd & 0xffff;
	r->swd = __r.swd & 0xffff;
	r->twd = __r.twd & 0xffff;
	r->fip = __r.fip;
	r->fcs = __r.fcs & 0xffff;
	r->foc = ((__r.fcs >> 16) & 0x07ff) | 0xd800;
	r->fop = __r.foo;
	r->fos = __r.fos & 0xffff;

	/* Store the MMX registers as well; the top 16 bits of the FPU
	 * registers (the exponent) which aren't used for MMX but set to 1 bits
	 * by MMX write instructions are provided for ease of use/sanity
	 * checking.
	 */
	r->mm0 = *(uint64_t *)mmx_regp;
	r->top0 = *(mmx_regp + 4);
	r->mm1 = *(uint64_t *)(mmx_regp + 5);
	r->top1 = *(mmx_regp + 9);
	r->mm2 = *(uint64_t *)(mmx_regp + 10);
	r->top2 = *(mmx_regp + 14);
	r->mm3 = *(uint64_t *)(mmx_regp + 15);
	r->top3 = *(mmx_regp + 19);
	r->mm4 = *(uint64_t *)(mmx_regp + 20);
	r->top4 = *(mmx_regp + 24);
	r->mm5 = *(uint64_t *)(mmx_regp + 25);
	r->top5 = *(mmx_regp + 29);
	r->mm6 = *(uint64_t *)(mmx_regp + 30);
	r->top6 = *(mmx_regp + 34);
	r->mm7 = *(uint64_t *)(mmx_regp + 35);
	r->top7 = *(mmx_regp + 39);

	PTRACE_ERR_CLEAR(p);
	return 0;
}

int ptrace_set_mmx_registers(
	struct ptrace_context *p,
	struct ptrace_mmx_registers *r
) {
	struct user_fpregs_struct __r;
	uint16_t *mmx_regp = (uint16_t *) __r.st_space;

	/* XXX: we might want to error when r->foc has the five
	 * bits set to not 11011b...
	 */
	__r.cwd = r->cwd | 0xffff0000;
	__r.swd = r->swd | 0xffff0000;
	__r.twd = r->twd | 0xffff0000;
	__r.fip = r->fip;
	__r.fcs = ((r->foc & 0x7ff) << 16) | r->fcs;
	__r.foo = r->fop;
	__r.fos = r->fos;

	*(uint64_t *)mmx_regp = r->mm0;
	*(mmx_regp + 4) = r->top0;
	*(uint64_t *)(mmx_regp + 5) = r->mm1;
	*(mmx_regp + 9) = r->top1;
	*(uint64_t *)(mmx_regp + 10) = r->mm2;
	*(mmx_regp + 14) = r->top2;
	*(uint64_t *)(mmx_regp + 15) = r->mm3;
	*(mmx_regp + 19) = r->top3;
	*(uint64_t *)(mmx_regp + 20) = r->mm4;
	*(mmx_regp + 24) = r->top4;
	*(uint64_t *)(mmx_regp + 25) = r->mm5;
	*(mmx_regp + 29) = r->top5;
	*(uint64_t *)(mmx_regp + 30) = r->mm6;
	*(mmx_regp + 34) = r->top6;
	*(uint64_t *)(mmx_regp + 35) = r->mm7;
	*(mmx_regp + 39) = r->top7;

	if ( ptrace(PTRACE_SETFPREGS, p->tid, NULL, &__r) == -1 ) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return -1;
	}

	PTRACE_ERR_CLEAR(p);
	return 0;
}
#endif	/* USE_MMX */

#ifdef USE_SSE
int ptrace_get_sse_registers(
	struct ptrace_context *p,
	struct ptrace_sse_registers *r
) {

}
#endif

int ptrace_get_debug_registers(
	struct ptrace_context *p,
	struct ptrace_debug_registers *r
) {
	struct ptrace_debug_registers __r;

	if ( ptrace_get_dr0(p, &__r.dr0) == -1 ||
	     ptrace_get_dr1(p, &__r.dr1) == -1 ||
	     ptrace_get_dr2(p, &__r.dr2) == -1 ||
	     ptrace_get_dr3(p, &__r.dr3) == -1 ||
	     ptrace_get_dr6(p, &__r.dr6) == -1 ||
	     ptrace_get_dr7(p, &__r.dr7) == -1 )
		return -1;

	r->dr0 = __r.dr0;
	r->dr1 = __r.dr1;
	r->dr2 = __r.dr2;
	r->dr3 = __r.dr3;
	r->dr6 = __r.dr6;
	r->dr7 = __r.dr7;

	return 0;
}

int ptrace_set_debug_registers(
	struct ptrace_context *p,
	struct ptrace_debug_registers *r
) {
	int restore = -1;
	struct ptrace_debug_registers old;

	/* We store the old debug register context, as we will make an attempt
	 * to restore the old context when setting the new one fails.
	 * The primary reason failures happen on linux is because of the use of
	 * insane values for the debug registers.
	 */
	if ( ptrace_get_debug_registers(p, &old) == -1 )
		return -1;

	/* Try to set all debug registers DR0 through DR7.
	 * On error, we set restore and break from the construct, continuing
	 * with the restore code.
	 */
	if ( ptrace_set_dr0(p, r->dr0) == -1 )
		restore = 0;
	else if ( ptrace_set_dr1(p, r->dr1) == -1 )
		restore = 1;
	else if ( ptrace_set_dr2(p, r->dr2) == -1 )
		restore = 2;
	else if ( ptrace_set_dr3(p, r->dr3) == -1 )
		restore = 3;
	else if ( ptrace_set_dr6(p, r->dr6) == -1 )
		restore = 6;
	else if ( ptrace_set_dr7(p, r->dr7) == -1 )
		restore = 7;

	/* Restore the old debug register context.
	 * XXX: In case this fails, we need to return an error denoting that
	 * the state of the remote process has been ruined.
	 */
	switch (restore) {
	case 7:
		ptrace_set_dr6(p, old.dr6);
	case 6:
		ptrace_set_dr3(p, old.dr3);
	case 3:
		ptrace_set_dr2(p, old.dr2);
	case 2:
		ptrace_set_dr1(p, old.dr1);
	case 1:
		ptrace_set_dr0(p, old.dr0);
	case 0:
		return -1;
	default:
		break;
	}
	
	return 0;
}

/* Macro facilitating function definitions for access to non segment register
 * elements of struct user_regs_struct.
 */
#define DEFINE_PTRACE_GET_REG(r)					\
int ptrace_get_##r(struct ptrace_context *p, uint32_t *r)		\
{									\
	long ret;							\
									\
	errno = 0;							\
	ret = ptrace(							\
		PTRACE_PEEKUSER,					\
		p->tid,							\
		offsetof(struct user_regs_struct, r),			\
		NULL							\
	);								\
									\
	if (ret == -1 && errno != 0) {					\
		PTRACE_ERR_SET_EXTERNAL(p);				\
		return -1;						\
	}								\
									\
	*r = (uint32_t) ret;						\
									\
	PTRACE_ERR_CLEAR(p);						\
	return 0;							\
}

/* Macro facilitating function definitions for access to segment register
 * elements of struct user_regs_struct.
 */
#define DEFINE_PTRACE_GET_SEG_REG(r)					\
int ptrace_get_##r(struct ptrace_context *p, uint32_t *r)		\
{									\
	long ret;							\
									\
	errno = 0;							\
	ret = ptrace(							\
		PTRACE_PEEKUSER,					\
		p->tid,							\
		offsetof(struct user_regs_struct, x##r),		\
		NULL							\
	);								\
									\
	if (ret == -1 && errno != 0) {					\
		PTRACE_ERR_SET_EXTERNAL(p);				\
		return -1;						\
	}								\
									\
	*r = (uint32_t) ret;						\
									\
	PTRACE_ERR_CLEAR(p);						\
	return 0;							\
}

/* Macro facilitating function definitions for access to debug register
 * elements of struct user.
 */
#define DEFINE_PTRACE_GET_DEBUG_REG(num)				\
int ptrace_get_dr##num(struct ptrace_context *p, uint32_t *dr##num)	\
{									\
	long ret;							\
									\
	errno = 0;							\
	ret = ptrace(							\
		PTRACE_PEEKUSER,					\
		p->tid,							\
		offsetof(struct user, u_debugreg[num]),			\
		NULL							\
	);								\
									\
	if (ret == -1 && errno != 0) {					\
		PTRACE_ERR_SET_EXTERNAL(p);				\
		return -1;						\
	}								\
									\
	*dr##num = (uint32_t) ret;					\
									\
	PTRACE_ERR_CLEAR(p);						\
	return 0;							\
}

#define DEFINE_PTRACE_SET_REG(r)					\
int ptrace_set_##r(struct ptrace_context *p, uint32_t r)		\
{									\
	long ret;							\
									\
	ret = ptrace(							\
		PTRACE_POKEUSER,					\
		p->tid,							\
		offsetof(struct user_regs_struct, r),			\
		(void *) r						\
	);								\
									\
	if (ret == -1) {						\
		PTRACE_ERR_SET_EXTERNAL(p);				\
		return -1;						\
	}								\
									\
	PTRACE_ERR_CLEAR(p);						\
	return 0;							\
}

#define DEFINE_PTRACE_SET_SEG_REG(r)					\
int ptrace_set_##r(struct ptrace_context *p, uint32_t r)		\
{									\
	long ret;							\
									\
	ret = ptrace(							\
		PTRACE_POKEUSER,					\
		p->tid,							\
		offsetof(struct user_regs_struct, x##r),		\
		(void *) r						\
	);								\
									\
	if (ret == -1) {						\
		PTRACE_ERR_SET_EXTERNAL(p);				\
		return -1;						\
	}								\
									\
	PTRACE_ERR_CLEAR(p);						\
	return 0;							\
}

#define DEFINE_PTRACE_SET_DEBUG_REG(num)				\
int ptrace_set_dr##num(struct ptrace_context *p, uint32_t dr##num)	\
{									\
	long ret;							\
									\
	ret = ptrace(							\
		PTRACE_POKEUSER,					\
		p->tid,							\
		offsetof(struct user, u_debugreg[num]),			\
		(void *) dr##num					\
	);								\
									\
	if (ret == -1) {						\
		PTRACE_ERR_SET_EXTERNAL(p);				\
		return -1;						\
	}								\
									\
	PTRACE_ERR_CLEAR(p);						\
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
DEFINE_PTRACE_GET_REG(orig_eax)
DEFINE_PTRACE_GET_REG(eflags)

DEFINE_PTRACE_GET_SEG_REG(cs)
DEFINE_PTRACE_GET_SEG_REG(ds)
DEFINE_PTRACE_GET_SEG_REG(es)
DEFINE_PTRACE_GET_SEG_REG(fs)
DEFINE_PTRACE_GET_SEG_REG(gs)
DEFINE_PTRACE_GET_SEG_REG(ss)

DEFINE_PTRACE_GET_DEBUG_REG(0)
DEFINE_PTRACE_GET_DEBUG_REG(1)
DEFINE_PTRACE_GET_DEBUG_REG(2)
DEFINE_PTRACE_GET_DEBUG_REG(3)
DEFINE_PTRACE_GET_DEBUG_REG(6)
DEFINE_PTRACE_GET_DEBUG_REG(7)

DEFINE_PTRACE_SET_REG(eax)
DEFINE_PTRACE_SET_REG(ebx)
DEFINE_PTRACE_SET_REG(ecx)
DEFINE_PTRACE_SET_REG(edx)
DEFINE_PTRACE_SET_REG(esi)
DEFINE_PTRACE_SET_REG(edi)
DEFINE_PTRACE_SET_REG(esp)
DEFINE_PTRACE_SET_REG(ebp)
DEFINE_PTRACE_SET_REG(eip)
DEFINE_PTRACE_SET_REG(orig_eax)
DEFINE_PTRACE_SET_REG(eflags)

DEFINE_PTRACE_SET_SEG_REG(cs)
DEFINE_PTRACE_SET_SEG_REG(ds)
DEFINE_PTRACE_SET_SEG_REG(es)
DEFINE_PTRACE_SET_SEG_REG(fs)
DEFINE_PTRACE_SET_SEG_REG(gs)
DEFINE_PTRACE_SET_SEG_REG(ss)

DEFINE_PTRACE_SET_DEBUG_REG(0)
DEFINE_PTRACE_SET_DEBUG_REG(1)
DEFINE_PTRACE_SET_DEBUG_REG(2)
DEFINE_PTRACE_SET_DEBUG_REG(3)
DEFINE_PTRACE_SET_DEBUG_REG(6)
DEFINE_PTRACE_SET_DEBUG_REG(7)
