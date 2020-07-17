int __secure_computing(const struct seccomp_data *sd)
{
        int mode = current->seccomp.mode;
        int this_syscall;
        ...
        this_syscall = sd ? sd->nr :
                syscall_get_nr(current, task_pt_regs(current));
        switch (mode) {
        case SECCOMP_MODE_STRICT:
                __secure_computing_strict(this_syscall);  /* may call do_exit */
                return 0;
        case SECCOMP_MODE_FILTER:
                return __seccomp_filter(this_syscall, sd, false);
        ...
        }
}

// SECCOMP_MODE_STRICT
static int mode1_syscalls[] = {
    __NR_seccomp_read, __NR_seccomp_write, __NR_seccomp_exit, __NR_seccomp_sigreturn,
    0, /* null terminated */
};
#ifdef CONFIG_COMPAT
static int mode1_syscalls_32[] = {
    __NR_seccomp_read_32, __NR_seccomp_write_32, __NR_seccomp_exit_32, __NR_seccomp_sigreturn_32,
    0, /* null terminated */
};
#endif
int __secure_computing(int this_syscall)
{
    int mode = current->seccomp.mode;
    int exit_sig = 0;
    int *syscall;
    u32 ret;
    switch (mode) {
    case SECCOMP_MODE_STRICT:
        syscall = mode1_syscalls;
#ifdef CONFIG_COMPAT
        if (is_compat_task())
        syscall = mode1_syscalls_32;
#endif
        do {
            if (*syscall == this_syscall)
                return 0;
        } while (*++syscall);
        exit_sig = SIGKILL;
        ret = SECCOMP_RET_KILL;
        break;
        ...
}

// SECCOMP_MORE_FILTER
