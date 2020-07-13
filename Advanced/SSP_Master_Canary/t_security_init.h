static void
security_init(void)
{
    /* Set up the stack checker's canary.  */
    uintptr_t stack_chk_guard = _dl_setup_stack_chk_guard(_dl_random);
#ifdef THREAD_SET_STACK_GUARD
    THREAD_SET_STACK_GUARD(stack_chk_guard);
#else
    __stack_chk_guard = stack_chk_guard;
#endif