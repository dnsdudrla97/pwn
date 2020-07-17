// gcc -o seccomp seccomp.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/prctl.h>
#include <linux/seccomp.h>
#include <linux/filter.h>
#include <linux/unistd.h>
#include <linux/audit.h>
#include <sys/mman.h>

int syscall_filter() {
    #define syscall_nr (offsetof(struct seccomp_data, nr))
    #define arch_nr (offsetof(struct seccomp_data, arch))
    
    /* architecture x86_64 */
    #define REG_SYSCALL REG_RAX
    #define ARCH_NR AUDIT_ARCH_X86_64
    struct sock_filter filter[] = {
        /* Validate architecture. */
        BPF_STMT(BPF_LD+BPF_W+BPF_ABS, arch_nr),
        BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, ARCH_NR, 1, 0),
        BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_KILL),
        /* Get system call number. */
        BPF_STMT(BPF_LD+BPF_W+BPF_ABS, syscall_nr),
        /* List allowed syscalls. */
        BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, __NR_rt_sigreturn, 0, 5),
        BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, __NR_open, 0, 4),
		BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, __NR_openat, 0, 3),
        BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, __NR_execve, 0, 2),
        BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, __NR_execveat, 0, 1),
        BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_KILL),
        BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW),
        };
    
    struct sock_fprog prog = {
    .len = (unsigned short)(sizeof(filter)/sizeof(filter[0])),
    .filter = filter,
        };
    if ( prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) == -1 ) {
        perror("prctl(PR_SET_NO_NEW_PRIVS)\n");
        return -1;
        }
    
    if ( prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog) == -1 ) {
        perror("Seccomp filter error\n");
        return -1;
        }
    return 0;
}
int main(int argc, char* argv[])
{
    void (*sc)();
    unsigned char *shellcode;
    shellcode = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    read(0, shellcode, 1024);
    syscall_filter();
    sc = (void *)shellcode;
    sc();
    return 0;
}