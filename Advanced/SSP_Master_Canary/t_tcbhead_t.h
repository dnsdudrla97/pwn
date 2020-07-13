typedef struct
{
  void *tcb; /* Pointer to the TCB.  Not necessarily the
			            thread descriptor used by libpthread.  */
  dtv_t *dtv;
  void *self; /* Pointer to the thread descriptor.  */
  int multiple_threads;
  uintptr_t sysinfo;
  uintptr_t stack_guard;
  uintptr_t pointer_guard;
  int gscope_flag;
#ifndef __ASSUME_PRIVATE_FUTEX
  int private_futex;
#else
  int __glibc_reserved1;
#endif
  /* Reservation of some values for the TM ABI.  */
  void *__private_tm[4];
  /* GCC split stack support.  */
  void *__private_ss;
} tcbhead_t;