#define THREAD_COPY_STACK_GUARD(descr) \
    ((descr)->header.stack_guard = THREAD_GETMEM(THREAD_SELF, header.stack_guard))
int __pthread_create_2_1(pthread_t *newthread, const pthread_attr_t *attr,
                         void *(*start_routine)(void *), void *arg)
{
    ...
#ifdef THREAD_COPY_STACK_GUARD
        THREAD_COPY_STACK_GUARD(pd);
#endif
/* Copy the pointer guard value.  */
#ifdef THREAD_COPY_POINTER_GUARD
    THREAD_COPY_POINTER_GUARD(pd);
#endif
/* Verify the sysinfo bits were copied in allocate_stack if needed.  */
#ifdef NEED_DL_SYSINFO
    CHECK_THREAD_SYSINFO(pd);
#endif
    ...
}