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
#include <stdlib.h>
#include <stddef.h>
#include <libptrace.h>
#include <libptrace_x86.h>

static int __ptrace_call(struct ptrace_context *p, void *code, long *retval);

#if defined(__i386__)
int
ptrace_get_program_ctr(struct ptrace_context *p, ptrace_x86_register_t *r)
{
	return ptrace_get_eip(p, r);
}

int
ptrace_set_program_ctr(struct ptrace_context *p, ptrace_x86_register_t r)
{
	return ptrace_set_eip(p, r);
}

int
ptrace_get_stack_ptr(struct ptrace_context *p, ptrace_x86_register_t *r)
{
	return ptrace_get_esp(p, r);
}

int
ptrace_set_stack_ptr(struct ptrace_context *p, ptrace_x86_register_t r)
{
	return ptrace_set_esp(p, r);
}

int
ptrace_get_base_ptr(struct ptrace_context *p, ptrace_x86_register_t *r)
{
	return ptrace_get_ebp(p, r);
}

int
ptrace_set_base_ptr(struct ptrace_context *p, ptrace_x86_register_t r)
{
	return ptrace_set_ebp(p, r);
}

#ifdef __linux__
int
ptrace_get_orig_acc(struct ptrace_context *p, ptrace_x86_register_t *r)
{
	return ptrace_get_orig_eax(p, r);
}

int ptrace_set_orig_acc(struct ptrace_context *p, ptrace_x86_register_t r)
{
	return ptrace_set_orig_eax(p, r);
}
#endif

int
ptrace_get_acc(struct ptrace_context *p, ptrace_x86_register_t *r)
{
	return ptrace_get_eax(p, r);
}

int
ptrace_set_acc(struct ptrace_context *p, ptrace_x86_register_t r)
{
	return ptrace_set_eax(p, r);
}

#elif defined(__x86_64__)
int
ptrace_get_program_ctr(struct ptrace_context *p, ptrace_x86_register_t *r)
{
	return ptrace_get_rip(p, r);
}

int
ptrace_set_program_ctr(struct ptrace_context *p, ptrace_x86_register_t r)
{
	return ptrace_set_rip(p, r);
}

int
ptrace_get_stack_ptr(struct ptrace_context *p, ptrace_x86_register_t *r)
{
	return ptrace_get_rsp(p, r);
}

int
ptrace_set_stack_ptr(struct ptrace_context *p, ptrace_x86_register_t r)
{
	return ptrace_set_rsp(p, r);
}

int
ptrace_get_base_ptr(struct ptrace_context *p, ptrace_x86_register_t *r)
{
	return ptrace_get_rbp(p, r);
}

int
ptrace_set_base_ptr(struct ptrace_context *p, ptrace_x86_register_t r)
{
	return ptrace_set_rbp(p, r);
}

int
ptrace_get_orig_acc(struct ptrace_context *p, ptrace_x86_register_t *r)
{
	return ptrace_get_orig_rax(p, r);
}

int ptrace_set_orig_acc(struct ptrace_context *p, ptrace_x86_register_t r)
{
	return ptrace_set_orig_rax(p, r);
}

int
ptrace_get_acc(struct ptrace_context *p, ptrace_x86_register_t *r)
{
	return ptrace_get_rax(p, r);
}

int
ptrace_set_acc(struct ptrace_context *p, ptrace_x86_register_t r)
{
	return ptrace_set_rax(p, r);
}
#else
#error "libptrace is not usable with this architecture."
#endif

int
ptrace_altstack_init(struct ptrace_context *pctx,
                     struct ptrace_altstack *stack, size_t size)
{
	void *__stack;

	size = ptrace_altstack_align(size);

#ifdef __linux__
	/* On linux, we mmap() alternative stacks with some flags well
	 * suited for stacks.
	 */
	__stack = ptrace_mmap(pctx, NULL, size,
	                      PROT_READ | PROT_WRITE,
	                      MAP_GROWSDOWN | MAP_ANONYMOUS | MAP_PRIVATE,
	                      -1, 0);
	if (__stack == MAP_FAILED)
		return -1;
#else
	if ( (__stack = ptrace_malloc(pctx, size)) == NULL)
		return -1;
#endif
	
	stack->base = __stack + size;
	stack->size = size;
	stack->stack_ptr = (ptrace_x86_register_t) stack->base;
	stack->base_ptr = (ptrace_x86_register_t) stack->base;
	stack->flags = PTRACE_ALTSTACK_NONE;

	return 0;
}

size_t ptrace_altstack_align(size_t size)
{
#ifdef WIN32
	/* Windows function calls do not like a misaligned stack pointer for
	 * some reason, so we align the stack to a multiple of 4 bytes.
         */
	return	size + (4 - size % 4);
#else
	return	size;
#endif
}

int
ptrace_altstack_current(struct ptrace_context *pctx,
                        struct ptrace_altstack *stack)
{
	ptrace_x86_register_t stack_ptr, base_ptr;

	if (ptrace_get_stack_ptr(pctx, &stack_ptr) == -1)
		return -1;

	if (ptrace_get_base_ptr(pctx, &base_ptr) == -1)
		return -1;

	stack->base = pctx->stack.base;
	stack->size = pctx->stack.size;
	stack->stack_ptr = stack_ptr;
	stack->base_ptr = base_ptr;
	stack->flags = pctx->stack.flags;

	return 0;
}

int
ptrace_altstack_switch(struct ptrace_context *pctx,
                       struct ptrace_altstack *stack,
		       struct ptrace_altstack *old_stack)
{
	struct ptrace_altstack __old_stack;

	/* Optionally save the previous stack we were using. */
	if (old_stack != NULL)
		if (ptrace_altstack_current(pctx, &__old_stack) == -1)
			return -1;

	/* Initialize the new stack. */
	if (ptrace_set_stack_ptr(pctx, stack->stack_ptr) == -1)
		return -1;

	/* XXX: fatal error occured... */
        if (ptrace_set_base_ptr(pctx, stack->base_ptr) == -1)
                return -1;

	/* Everything worked out.  Update the process context stack. */
	pctx->stack.base = stack->base;
	pctx->stack.size = stack->size;
	pctx->stack.flags = stack->flags;

	/* And write out the old_stack values if requested. */
	if (old_stack != NULL) {
		old_stack->base = __old_stack.base;
		old_stack->size = __old_stack.size;
		old_stack->base_ptr = __old_stack.base_ptr;
		old_stack->stack_ptr = __old_stack.stack_ptr;
		old_stack->flags = __old_stack.flags;
	}

	return 0;
}

int
ptrace_altstack_destroy(struct ptrace_context *pctx,
                        struct ptrace_altstack *stack)
{
	/* We do not allow the original process stack to be destroyed. */
	if (stack->flags & PTRACE_ALTSTACK_ORIG) {
		PTRACE_ERR_SET_INTERNAL(pctx, PTRACE_ERR_ALTSTACK_ORIG);
		return -1;
	}

	/* We do not allow an altstack to be destroyed when the remote
	 * process is currently using it.
	 */
	if (pctx->stack.base == stack->base) {
		PTRACE_ERR_SET_INTERNAL(pctx, PTRACE_ERR_ALTSTACK_INUSE);
		return -1;
	}

#ifdef __linux__
	return ptrace_munmap(pctx, stack->base - stack->size, stack->size);
#else
	return ptrace_free(pctx, stack->base - stack->size);
#endif
}

int ptrace_push16(struct ptrace_context *pctx, uint16_t word)
{
	ptrace_x86_register_t stack_ptr;

	if (ptrace_get_stack_ptr(pctx, &stack_ptr) == -1)
		return -1;

	if (ptrace_set_stack_ptr(pctx, stack_ptr - 2) == -1)
		return -1;

	if (ptrace_write(pctx, (void *)(stack_ptr - 2), &word, 2) == -1) {
		/* XXX: handle double fault */
		ptrace_set_stack_ptr(pctx, stack_ptr);
		return -1;
	}

	return 0;
}

int ptrace_push32(struct ptrace_context *pctx, uint32_t dword)
{
	ptrace_x86_register_t stack_ptr;

	if (ptrace_get_stack_ptr(pctx, &stack_ptr) == -1)
		return -1;

	if (ptrace_set_stack_ptr(pctx, stack_ptr - 4) == -1)
		return -1;

	if (ptrace_write(pctx, (void *)(stack_ptr - 4), &dword, 4) == -1) {
		/* XXX: handle double fault */
		ptrace_set_stack_ptr(pctx, stack_ptr);
		return -1;
	}

	return 0;
}

int ptrace_pop16(struct ptrace_context *pctx, uint16_t *word)
{
	uint16_t __word;
	ptrace_x86_register_t stack_ptr;

	if (ptrace_get_stack_ptr(pctx, &stack_ptr) == -1)
		return -1;

	if (ptrace_read(pctx, (void *)&__word, (void *)stack_ptr, 2) == -1)
		return -1;

	if (ptrace_set_stack_ptr(pctx, stack_ptr + 2) == -1)
		return -1;

	*word = __word;
	return 0;
}

int ptrace_pop32(struct ptrace_context *pctx, uint32_t *dword)
{
	uint32_t __dword;
	ptrace_x86_register_t stack_ptr;

	if (ptrace_get_stack_ptr(pctx, &stack_ptr) == -1)
		return -1;

	if (ptrace_read(pctx, (void *)&__dword, (void *)stack_ptr, 4) == -1)
		return -1;

	if (ptrace_set_stack_ptr(pctx, stack_ptr + 4) == -1)
		return -1;

	*dword = __dword;
	return 0;
}

/* Internal helper function which locates a codepage we can write to.
 * This is often allowed, but there can be exceptions to this, such as
 * a high memory range mapped COMPAT_VDSO page, which disallows writes,
 * due to the lack of COW implementation.
 */
ptrace_x86_register_t
__ptrace_get_writeable_codepage(
	struct ptrace_registers *regs,
	int page_size
) {
#if 0
#ifdef __linux__
	ptrdiff_t elf_header_base = (ptrdiff_t) ELF_HEADER_BASE;

	/* In case we have a COMPAT_VDSO page, we're in trouble.
	 * For now we return the ELF base, until someone comes up with
	 * something more elegant *sigh*.
	 */
	if (regs->eip >> 16 == 0xFFFF)
		return elf_header_base - elf_header_base % page_size;
#endif
#endif
#if defined(__i386__)
	return regs->eip - regs->eip % page_size;
#elif defined(__x86_64__)
	return regs->rip - regs->rip % page_size;
#endif
}

/* Internal function used to execute a payload which fits in a single page.
 *
 * It loads the payload into an executable page in the remote process space,
 * inserts a breakpoint afterwards, and sets the remote instruction pointer
 * to execute the payload.
 * Afterwards, if retval is unequal to NULL, the return value register is
 * saved here, and the original state of the process is restored.
 */
int __ptrace_run_code(struct ptrace_context *p, void *code,
                      size_t len, long *retval)
{
	uint8_t *__oldcode;
	int page_size, __retval;
	struct ptrace_registers regs;
	struct ptrace_fpu_state fpstate;
	ptrace_x86_register_t pc, new_pc;

	/* Test if the pagesize is large enough to accomodate our code.
	 * We reserve space for an 'int 3h' instruction as well.
	 */
	if (ptrace_get_pagesize(p, &page_size) == -1)
		goto out;

	if (page_size < len + 1) {
		PTRACE_ERR_SET_INTERNAL(p, PTRACE_ERR_PAGESIZE);
		goto out;
	}

	/* We want to access individual registers, to we get them */
	if (ptrace_get_registers(p, &regs) == -1)
		goto out;

	/* We're not interested in individual FP registers, so we get
	 * the FPU state and be done with it.
	 */
	if (ptrace_save_fpu_state(p, &fpstate) == -1)
		goto out;

	/* Let eip point to the beginning of the code page */
	pc = __ptrace_get_writeable_codepage(&regs, page_size);

	/* Make a backup of the old code there */
	__oldcode = (uint8_t *) malloc(len + 1);
	if (__oldcode == NULL) {
		PTRACE_ERR_SET_EXTERNAL(p);
		goto out;
	}

	if (ptrace_read(p, __oldcode, (void *)pc, len + 1) == -1)
		goto out_free;

	/* Inject the payload, and a breakpoint. */
	if (ptrace_write(p, (void *)pc, code, len) == -1)
		goto out_free;
	
	if (ptrace_write(p, (void *)pc + len, "\xcc", 1) == -1)
		goto out_write;

	/* Now set $eip in the remote process to this code */
	if (ptrace_set_program_ctr(p, pc) == -1)
		goto out_write;

#ifdef __linux__
	/* On linux, orig_eax is used to restart interruped syscalls.
	 * We do not want to risk the kernel subtracting 2 from $eip, and
	 * which it will only do if orig_[er]ax >= 0.  We store it, and set
	 * it to -1 for our purposes.
	 */
	if (ptrace_set_orig_acc(p, -1) == -1)
		goto out_state;
#endif

	/* Finally wait for a SIGTRAP to signal the callee returned.
	 * In theory signals which cause a SIGTRAP themselves could
	 * cause ptrace_wait_signal() to return before we trapped on our
	 * own inserted breakpoint, so we loop in this case.
	 */
	do {
		if (ptrace_wait_breakpoint(p) == -1)
			goto out_state;

		if (ptrace_get_program_ctr(p, &new_pc) == -1 )
			goto out_state;
	} while (new_pc != pc + len + 1);

	/* Store the return value if we want one */
	if (retval &&
	    ptrace_get_acc(p, (ptrace_x86_register_t *)&__retval) == -1)
		goto out_state;

	/* Restore the code, register and FPU state */
	/* XXX: handle errors */
	ptrace_write(p, (void *)pc, __oldcode, len + 1);
	ptrace_set_registers(p, &regs);
	ptrace_load_fpu_state(p, &fpstate);

	if (retval)
		*retval = __retval;
	
	return 0;

out_state:
	/* XXX: double fault */
	ptrace_set_registers(p, &regs);
	ptrace_load_fpu_state(p, &fpstate);
out_write:
	/* XXX: double fault */
	ptrace_write(p, (void *)pc, __oldcode, len + 1);
out_free:
	free(__oldcode);
out:
	return -1;
}

/* Execute a call instruction to function in the remote process, and stop
 * execution again afterwards.
 *
 * It is advisable to setup an alternative stack in the remote process before
 * using this function.  This might not be necessary, as depending on
 * consistency of data past the top of the stack is rather hazardous, unless
 * signal handling is disabled.
 *
 * It will completely restore the register and FPU states, and should not be
 * used to set specific registers, but a return value is stored in 'retval'.
 */
static int
__ptrace_call(struct ptrace_context *p, void *code, long *retval)
{
#if defined(__i386__)
	uint8_t __stub[] = {
		0xb8, 0x00, 0x00, 0x00, 0x00,	/* mov eax, 0 */
		0xff, 0xd0			/* call eax */
	};

	/* Patch the calling stub with the correct absolute address, and
	 * write out the calling code.
	 */
	*(uint32_t *)(__stub + 1) = (uint32_t)code;
#elif defined(__x86_64__)
	uint8_t __stub[] = {
		0x48, 0xb8, 0x00, 0x00, 0x00, 0x00,	/* mov rax, 0 */
		            0x00, 0x00, 0x00, 0x00,
		0x48, 0xff, 0xd0			/* call rax */
	};

	/* Patch the calling stub with the correct absolute address, and
	 * write out the calling code.
	 */
	*(uint64_t *)(__stub + 2) = (uint64_t)code;
#endif

	return __ptrace_run_code(p, __stub, sizeof(__stub), retval);
}

int ptrace_call_function(struct ptrace_context *p, void *code, long *retval)
{
	return __ptrace_call(p, code, retval);
}

int ptrace_call_procedure(struct ptrace_context *p, void *code)
{
	return __ptrace_call(p, code, NULL);
}
