#ifndef __LIBPTRACE_WRAPPER_H
#define __LIBPTRACE_WRAPPER_H

#include <stdio.h>
#include <sys/types.h>

pid_t waitpid_no_EINTR(pid_t pid, int *status, int options);
int fclose_no_EINTR(FILE *fp);

#endif
