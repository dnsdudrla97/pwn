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
#ifndef __LIBPTRACE_X86_H
#define __LIBPTRACE_X86_H

#if defined(__i386__)
typedef uint16_t ptrace_x86_seg_register_t;
typedef uint32_t ptrace_x86_register_t;
#elif defined(__x86_64__)
typedef uint16_t ptrace_x86_seg_register_t;
typedef uint64_t ptrace_x86_register_t;
#else
#error "libptrace is not usable with this architecture."
#endif

/* This header file defines common functions for all x86 families. */

/* Defined in other header files, but just forward declare it to prevent
 * preprocessing mess.
 */
struct ptrace_context;
struct ptrace_cpu_state;
struct ptrace_fpu_state;
struct ptrace_altstack;
struct ptrace_debug_registers;

/* According to the Intel Manual Volume 3 15.2.2:
* <QUOTE>
* Debug registers DR4 and DR5 are reserved when debug extentions are enabled
* (when the DE flag in control register CR4 is set), and attempts to reference
* the DR4 and DR5 registers cause an invalid-opcode exception (#UD) to be
* generated. When debug extentions are not enabled (when the DE flag is
* clear), these registers are aliased to debug registers DR6 and DR7.
* </QUOTE>
*
* On linux we seem to be able to read debug registers DR4 and DR5 through
* using ptrace, but these will always be 0. Attempts to write them result in
* EIO.
* As we're unable to read CR4 without ring0 access, we cannot implement the
* described aliasing in this library. Therefore, we omit functions allowing
* access to DR4 and DR5.
*/
struct ptrace_debug_registers {
       ptrace_x86_register_t dr0;
       ptrace_x86_register_t dr1;
       ptrace_x86_register_t dr2;
       ptrace_x86_register_t dr3;
       ptrace_x86_register_t dr6;
       ptrace_x86_register_t dr7;
};

/* Debug register read functions */
int ptrace_get_dr0(struct ptrace_context *, ptrace_x86_register_t *);
int ptrace_get_dr1(struct ptrace_context *, ptrace_x86_register_t *);
int ptrace_get_dr2(struct ptrace_context *, ptrace_x86_register_t *);
int ptrace_get_dr3(struct ptrace_context *, ptrace_x86_register_t *);
int ptrace_get_dr6(struct ptrace_context *, ptrace_x86_register_t *);
int ptrace_get_dr7(struct ptrace_context *, ptrace_x86_register_t *);

/* Debug register write functions */
int ptrace_set_dr0(struct ptrace_context *, ptrace_x86_register_t);
int ptrace_set_dr1(struct ptrace_context *, ptrace_x86_register_t);
int ptrace_set_dr2(struct ptrace_context *, ptrace_x86_register_t);
int ptrace_set_dr3(struct ptrace_context *, ptrace_x86_register_t);
int ptrace_set_dr6(struct ptrace_context *, ptrace_x86_register_t);
int ptrace_set_dr7(struct ptrace_context *, ptrace_x86_register_t);

/* Floating point registers structure.
 *
 * Note that according to Intel Manual Volume 1 Section 8.1.8 only the
 * last 3 bits of the first opcode byte are stored in the Opcode register,
 * as the first 5 bits are always 11011b.
 *
 * Please note that libptrace adds these 5 bits to the value of 'fop' in the
 * ptrace_fpu_registers structure.
 */
struct ptrace_fpu_registers {
	uint16_t cwd;		/* control word */
	uint16_t swd;		/* status word */
	uint16_t twd;		/* tag word */
	uint16_t foc;		/* last instruction opcode */
	uint32_t fip;		/* last instruction pointer */
	uint16_t fcs;		/* last instruction pointer segment */
	uint32_t fop;		/* last data operand pointer */
	uint16_t fos;		/* last data operand pointer segment */
	uint32_t st_space[20];	/* 8*10 bytes for each FP-reg = 80 bytes */
};

/* MMX registers structure.
 *
 * This is aliased to the FPU register state, but we name the registers
 * explicitly, as they are in fixed locations instead of stack based.
 *
 * This struct simply presents another way to look at the FPU state.
 */
struct ptrace_mmx_registers {
	uint16_t cwd;		/* control word */
	uint16_t swd;		/* status word */
	uint16_t twd;		/* tag word */
	uint16_t foc;		/* last instruction opcode */
	uint32_t fip;		/* last instruction pointer */
	uint16_t fcs;		/* last instruction pointer segment */
	uint32_t fop;		/* last data operand pointer */
	uint16_t fos;		/* last data operand pointer segment */

	uint64_t mm0;		/* MMX registers - MM0 through MM7 */
	uint64_t mm1;
	uint64_t mm2;
	uint64_t mm3;
	uint64_t mm4;
	uint64_t mm5;
	uint64_t mm6;
	uint64_t mm7;

	uint16_t top0;		/* Top 16 bit halves (exponents) */
	uint16_t top1;
	uint16_t top2;
	uint16_t top3;
	uint16_t top4;
	uint16_t top5;
	uint16_t top6;
	uint16_t top7;
};

/* SSE registers structure.
 *
 * This only presents the SSE specific registers, and not the full FPU state.
 */
struct ptrace_sse_registers {
	uint8_t	xmm0[16];	/* SSE registers - XMM0 through XMM7 */
	uint8_t	xmm1[16];
	uint8_t	xmm2[16];
	uint8_t	xmm3[16];
	uint8_t	xmm4[16];
	uint8_t	xmm5[16];
	uint8_t	xmm6[16];
	uint8_t	xmm7[16];
	uint32_t mxcsr;		/* SSE control and status register */
};

/* Alternative stack structure.
 * This is used for easy management of multiple stacks in a remote process.
 */
#define PTRACE_ALTSTACK_NONE	0
#define PTRACE_ALTSTACK_ORIG	1

struct ptrace_altstack {
	void			*base;
	size_t			size;
	ptrace_x86_register_t	stack_ptr;
	ptrace_x86_register_t	base_ptr;
	unsigned int		flags;
};

/* State saving functions.
 *
 * These should not be used for anything but saving and restoring state, as the
 * interfaces can change in the kernel and depending on support for extended
 * FPU data and so on.
 */
int ptrace_save_cpu_state(struct ptrace_context *,
                          struct ptrace_cpu_state *);
int ptrace_load_cpu_state(struct ptrace_context *,
                          struct ptrace_cpu_state *);
int ptrace_save_fpu_state(struct ptrace_context *,
                          struct ptrace_fpu_state *);
int ptrace_load_fpu_state(struct ptrace_context *,
                          struct ptrace_fpu_state *);

/* The FPU access functions are defined here for now, as an architecture
 * might not have an FPU in the first place.
 */
int ptrace_get_fpu_registers(struct ptrace_context *,
                             struct ptrace_fpu_registers *);
int ptrace_set_fpu_registers(struct ptrace_context *,
                             struct ptrace_fpu_registers *);

int ptrace_get_mmx_registers(struct ptrace_context *,
                             struct ptrace_mmx_registers *);

int ptrace_get_debug_registers(struct ptrace_context *,
                               struct ptrace_debug_registers *);
int ptrace_set_debug_registers(struct ptrace_context *,
                               struct ptrace_debug_registers *);

/* x86 register functions */
int ptrace_get_cs(struct ptrace_context *p, uint16_t *cs);
int ptrace_get_ds(struct ptrace_context *p, uint16_t *ds);
int ptrace_get_es(struct ptrace_context *p, uint16_t *es);
int ptrace_get_ss(struct ptrace_context *p, uint16_t *ss);
int ptrace_get_fs(struct ptrace_context *p, uint16_t *fs);
int ptrace_get_gs(struct ptrace_context *p, uint16_t *gs);
int ptrace_get_eflags(struct ptrace_context *p, uint32_t *eflags);

int ptrace_set_cs(struct ptrace_context *p, uint16_t cs);
int ptrace_set_ds(struct ptrace_context *p, uint16_t ds);
int ptrace_set_es(struct ptrace_context *p, uint16_t es);
int ptrace_set_ss(struct ptrace_context *p, uint16_t ss);
int ptrace_set_fs(struct ptrace_context *p, uint16_t fs);
int ptrace_set_gs(struct ptrace_context *p, uint16_t gs);
int ptrace_set_eflags(struct ptrace_context *p, uint32_t eflags);

/* i386 register read functions */
int ptrace_get_eax(struct ptrace_context *p, uint32_t *eax);
int ptrace_get_ebx(struct ptrace_context *p, uint32_t *ebx);
int ptrace_get_ecx(struct ptrace_context *p, uint32_t *ecx);
int ptrace_get_edx(struct ptrace_context *p, uint32_t *edx);
int ptrace_get_esi(struct ptrace_context *p, uint32_t *esi);
int ptrace_get_edi(struct ptrace_context *p, uint32_t *edi);
int ptrace_get_ebp(struct ptrace_context *p, uint32_t *ebp);
int ptrace_get_esp(struct ptrace_context *p, uint32_t *esp);
int ptrace_get_eip(struct ptrace_context *p, uint32_t *eip);

/* i386 register write functions */
int ptrace_set_eax(struct ptrace_context *p, uint32_t eax);
int ptrace_set_ebx(struct ptrace_context *p, uint32_t ebx);
int ptrace_set_ecx(struct ptrace_context *p, uint32_t ecx);
int ptrace_set_edx(struct ptrace_context *p, uint32_t edx);
int ptrace_set_esi(struct ptrace_context *p, uint32_t esi);
int ptrace_set_edi(struct ptrace_context *p, uint32_t edi);
int ptrace_set_ebp(struct ptrace_context *p, uint32_t ebp);
int ptrace_set_esp(struct ptrace_context *p, uint32_t esp);
int ptrace_set_eip(struct ptrace_context *p, uint32_t eip);

/* x86_64 register read functions */
int ptrace_get_rax(struct ptrace_context *p, uint64_t *rax);
int ptrace_get_rbx(struct ptrace_context *p, uint64_t *rbx);
int ptrace_get_rcx(struct ptrace_context *p, uint64_t *rcx);
int ptrace_get_rdx(struct ptrace_context *p, uint64_t *rdx);
int ptrace_get_rsi(struct ptrace_context *p, uint64_t *rsi);
int ptrace_get_rdi(struct ptrace_context *p, uint64_t *rdi);
int ptrace_get_rbp(struct ptrace_context *p, uint64_t *rbp);
int ptrace_get_rsp(struct ptrace_context *p, uint64_t *rsp);
int ptrace_get_rip(struct ptrace_context *p, uint64_t *rip);

/* x86_64 register write functions */
int ptrace_set_rax(struct ptrace_context *p, uint64_t rax);
int ptrace_set_rbx(struct ptrace_context *p, uint64_t rbx);
int ptrace_set_rcx(struct ptrace_context *p, uint64_t rcx);
int ptrace_set_rdx(struct ptrace_context *p, uint64_t rdx);
int ptrace_set_rsi(struct ptrace_context *p, uint64_t rsi);
int ptrace_set_rdi(struct ptrace_context *p, uint64_t rdi);
int ptrace_set_rbp(struct ptrace_context *p, uint64_t rbp);
int ptrace_set_rsp(struct ptrace_context *p, uint64_t rsp);
int ptrace_set_rip(struct ptrace_context *p, uint64_t rip);

/* Alternative stack management functions. */
int ptrace_altstack_init(struct ptrace_context *pctx,
                         struct ptrace_altstack *stack, size_t size);
size_t ptrace_altstack_align(size_t size);
int ptrace_altstack_current(struct ptrace_context *pctx,
                            struct ptrace_altstack *stack);
int ptrace_altstack_switch(struct ptrace_context *pctx,
                           struct ptrace_altstack *stack,
                           struct ptrace_altstack *old_stack);
int ptrace_altstack_destroy(struct ptrace_context *pctx,
                           struct ptrace_altstack *stack);

/* Push values to the stack */
int ptrace_push16(struct ptrace_context *pctx, uint16_t word);
int ptrace_push32(struct ptrace_context *pctx, uint32_t dword);
int ptrace_pop16(struct ptrace_context *pctx, uint16_t *word);
int ptrace_pop32(struct ptrace_context *pctx, uint32_t *dword);

/* Call functions/procedures in the remote process */
int ptrace_call_function(struct ptrace_context *p,
                         void *code, long *retval);
int ptrace_call_procedure(struct ptrace_context *p, void *code);
int ptrace_call_nowait(struct ptrace_context *p, void *code);

#endif	/* !__LIBPTRACE_X86_H */
