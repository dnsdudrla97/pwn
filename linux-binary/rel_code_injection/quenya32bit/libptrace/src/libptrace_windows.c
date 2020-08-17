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

/* For now we define the Windows versions like this, so MingW will add
 * support for OpenThread().
 * Later we will improve compatibility and wrap OpenThread() nicely.
 *
 * TODO:
 * - inject DLLs into process space, do relocation etc.
 * - think on access modes, limit functionality if we can't get everything
 *   we might like.
 * - on windows support opening by pure PID instead of TID, as we might
 *   not be able to OpenThread() but possibly we can have OpenProcess().
 * - compatibility with Win9x
 */

/* Make sure we have DebugActiveProcessStop() */
#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <winbase.h>
#include <winnt.h>
#include <ntdef.h>
#include <ddk/ntapi.h>
#include "libptrace.h"

/* Prototype this function, as mingw lacks a header file for it */
ULONG WINAPI RtlNtStatusToDosError(NTSTATUS Status);

int ptrace_open(struct ptrace_context *p, ptrace_pid_t tid)
{
	return ptrace_attach(p, tid);
}

/* Open a process / thread given its identifier */
int ptrace_attach(struct ptrace_context *p, ptrace_pid_t tid)
{
	HANDLE ph, th;
	ptrace_pid_t pid;

	/* Initialize the error context first of all, as we might use it in
	 * this function.
	 */
	p->error.errmsg = NULL;
	PTRACE_ERR_CLEAR(p);

	/* We open the thread that the LWP id passed to this function refers
	 * to.
	 */
	th = OpenThread(
		THREAD_QUERY_INFORMATION |
		THREAD_GET_CONTEXT |
		THREAD_SET_CONTEXT |
		THREAD_SUSPEND_RESUME,
		FALSE,
		tid
	);
	if (th == NULL) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return -1;
	}

	/* Retrieve the process id of the process this thread belongs to. */
	if ( (pid = GetProcessIdOfThread(th)) == 0 ) {
		PTRACE_ERR_SET_EXTERNAL(p);
		CloseHandle(th);
		return -1;
	}

	/* Enable a debugger to attach to the process and debug it. */
	if ( DebugActiveProcess(pid) == 0 ) {
		PTRACE_ERR_SET_EXTERNAL(p);
		CloseHandle(th);
		return -1;
	}

	/* Make sure we do not kill the process on exit. */
	if ( DebugSetProcessKillOnExit(FALSE) == 0 ) {
		PTRACE_ERR_SET_EXTERNAL(p);
		CloseHandle(th);
		DebugActiveProcessStop(pid);
		return -1;
	}

	/* We open the process encapsulating this thread */
	ph = OpenProcess(
		PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ |
		PROCESS_VM_WRITE |
		PROCESS_VM_OPERATION,
		FALSE,
		pid
	);
	if (ph == NULL) {
		PTRACE_ERR_SET_EXTERNAL(p);
		CloseHandle(th);			/* XXX: might fail */
		DebugActiveProcessStop(pid);		/* XXX: error? */
		return -1;
	}

	/* Initialize the ptrace library context */
	p->tid = tid;
	p->pid = pid;
	p->thandle = th;
	p->phandle = ph;
	p->continue_status = -1;

	/* Initialize the current altstack as the original stack. */
	p->stack.flags = PTRACE_ALTSTACK_ORIG;

	return 0;
}

int ptrace_close(struct ptrace_context *p)
{
	return ptrace_detach(p);
}

/* Close the traced process given its context.
 *
 * Things get fairly ugly if this function fails, as part of the ptrace
 * context might be ruined at that time.
 * Not much we can do about it; if ptrace_detach() fails ptrace_context
 * will be in an inconsistent state.
 */
int ptrace_detach(struct ptrace_context *p)
{
	if (p->thandle != NULL) {
		if ( CloseHandle(p->thandle) == 0 ) {
			PTRACE_ERR_SET_EXTERNAL(p);
			return -1;
		} else {
			p->thandle = NULL;
		}
	}

	if (p->phandle != NULL) {
		if ( CloseHandle(p->phandle) == 0 ) {
			PTRACE_ERR_SET_EXTERNAL(p);
			return -1;
		} else {
			p->phandle = NULL;
		}
	}

	/* Make sure we handle a possible exception correctly. */
	if (p->continue_status != -1)
		ContinueDebugEvent(p->pid, p->tid, p->continue_status);

	/* Stop debugging the active process */
	if ( DebugActiveProcessStop(p->pid) == 0 ) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return -1;
	}

	if (p->error.errmsg != NULL)
		LocalFree(p->error.errmsg);

	PTRACE_ERR_CLEAR(p);
	return 0;
}

/* Write 'len' bytes from 'src' to the location 'dest' in the process
 * described in the ptrace_context 'p'.
 */
int ptrace_write(struct ptrace_context *p, void *dest, const void *src, size_t len)
{
	BOOL ret = WriteProcessMemory(p->phandle, dest, src, len, NULL);

	if (ret == 0) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return -1;
	} else {
		PTRACE_ERR_CLEAR(p);
		return 0;
	}
}

/* Read 'len' bytes from 'src' in the process
 * described in the ptrace_context 'p' to the location 'dest'.
 */
int ptrace_read(struct ptrace_context *p, void *dest, const void *src, size_t len)
{
	BOOL ret = ReadProcessMemory(p->phandle, src, dest, len, NULL);

	if (ret == 0) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return -1;
	} else {
		PTRACE_ERR_CLEAR(p);
		return 0;
	}
}

/* Allocate 'len' bytes somewhere in the process described in the
 * ptrace_context 'p'.
 */
void *ptrace_malloc(struct ptrace_context *p, size_t len)
{
	void *ret = VirtualAllocEx(
		p->phandle,
		NULL,
		len,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_EXECUTE_READWRITE
	);

	if (ret == NULL)
		PTRACE_ERR_SET_EXTERNAL(p);
	else
		PTRACE_ERR_CLEAR(p);
	
	return ret;
}

int ptrace_free(struct ptrace_context *p, void *mem)
{
	BOOL ret = VirtualFreeEx(p->phandle, mem, 0, MEM_RELEASE);

	if (ret == 0) {
		PTRACE_ERR_SET_EXTERNAL(p);
		return -1;
	} else {
		PTRACE_ERR_CLEAR(p);
		return 0;
	}
}

/* Error handling functions */

inline static char *chompA(char *str)
{
        size_t len = strlen(str);

        if (len > 0 && str[len - 1] == '\n') {
                if (len > 1 && str[len - 2] == '\r')
                        str[len - 2] = '\0';
	        str[len - 1] = '\0';
	}

	return str;
}

inline static wchar_t *chompW(wchar_t *str)
{
        size_t len = wcslen(str);

        if (len > 0 && str[len - 1] == L'\n') {
                if (len > 1 && str[len - 2] == L'\r')
                        str[len - 2] = L'\0';
	        str[len - 1] = L'\0';
	}

	return str;
}

const wchar_t *ptrace_errmsg16(struct ptrace_context *p)
{
        DWORD ret;

	if (p->error.errmsg != NULL) {
		LocalFree(p->error.errmsg);
		p->error.errmsg = NULL;
	}

	if (p->error.internal & PTRACE_ERR_FLAG_EXTERNAL) {
		ret = FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER  |
			FORMAT_MESSAGE_FROM_SYSTEM      |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			p->error.external,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(wchar_t *) (void *) &p->error.errmsg,
			1,
			NULL
		);

	        if (ret == 0)
			return NULL;		/* XXX: handle better */

		return chompW((wchar_t *) p->error.errmsg);
	}

	return NULL;
}

const char *ptrace_errmsg(struct ptrace_context *p)
{
        DWORD ret;

	if (p->error.errmsg != NULL) {
		LocalFree(p->error.errmsg);
		p->error.errmsg = NULL;
	}

	if (p->error.flags & PTRACE_ERR_FLAG_EXTERNAL) {
		ret = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER  |
			FORMAT_MESSAGE_FROM_SYSTEM      |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			p->error.external,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(char *) &p->error.errmsg,
			1,
			NULL
		);

	        if (ret == 0)
			return NULL;		/* XXX: handle better */

		return chompA((char *) p->error.errmsg);
	}

	return NULL;
}

int ptrace_get_pagesize(struct ptrace_context *p, int *page_size)
{
	SYSTEM_INFO si;

	GetSystemInfo(&si);
	*page_size = (int) si.dwPageSize;

	return 0;
}

int ptrace_wait_breakpoint(struct ptrace_context *p)
{
	BOOL ret;
	DEBUG_EVENT ev;

	if (p->continue_status != -1) {
		ret = ContinueDebugEvent(p->pid, p->tid, p->continue_status);
		p->continue_status = -1;	/* Don't retry on failure. */
		if (ret == 0) {
			PTRACE_ERR_SET_EXTERNAL(p);
			return -1;
		}
	}

	do {
		if ( WaitForDebugEvent(&ev, INFINITE) == 0 ) {
			PTRACE_ERR_SET_EXTERNAL(p);
			return -1;
		}

		if (ev.dwProcessId == p->pid && ev.dwThreadId == p->tid) {
			/* In case the process or thread exits, we report a
			 * failure.
			 */
			if (ev.dwDebugEventCode == EXIT_THREAD_DEBUG_EVENT ||
			    ev.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT) {
				PTRACE_ERR_SET_INTERNAL(p, PTRACE_ERR_EXITED);
				return -1;
			}

			/* If we have a breakpoint exception, we're set! */
			if (ev.dwDebugEventCode == EXCEPTION_DEBUG_EVENT &&
			    ev.u.Exception.ExceptionRecord.ExceptionCode ==
		    					EXCEPTION_BREAKPOINT) {
				break;
			}
		}

		/* If we did not find a debug exception, continue and have
		 * the host process handle the exception.
		 */
		if ( ContinueDebugEvent(ev.dwProcessId, ev.dwThreadId,
		                        DBG_EXCEPTION_NOT_HANDLED) == 0 ) {
			PTRACE_ERR_SET_EXTERNAL(p);
			return -1;
		}
	} while (1);

	/* Make sure we ignore breakpoint exceptions in the host process. */
	p->continue_status = DBG_CONTINUE;
	PTRACE_ERR_CLEAR(p);
	return 0;
}

/* Windows compatibility functions
 */
DWORD WINAPI GetProcessIdOfThread(HANDLE hThread)
{
	NTSTATUS Status;
	THREAD_BASIC_INFORMATION tbi;

	Status = NtQueryInformationThread(
			hThread,
			ThreadBasicInformation,
			&tbi,
			sizeof(tbi),
			NULL
	);

	if ( !NT_SUCCESS(Status) ) {
		SetLastError(RtlNtStatusToDosError(Status));
		return 0;
	}

	return (DWORD)tbi.ClientId.UniqueProcess;
}

ptrace_library_handle_t
ptrace_library_load(struct ptrace_context *pctx, const char *library)
{
	return ptrace_load_library(pctx, library);
}

ptrace_function_ptr_t
ptrace_library_get_function_addr(struct ptrace_context *pctx,
                                 ptrace_library_handle_t handle,
				 const char *function)
{
	return ptrace_get_proc_address(pctx, handle, function);
}

int
ptrace_library_unload(struct ptrace_context *pctx,
                      ptrace_library_handle_t handle)
{
	BOOL ret = ptrace_free_library(pctx, handle);

	return ret != 0 ? 0 : -1;
}
