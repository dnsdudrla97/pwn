#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t waitpid_no_EINTR(pid_t pid, int *status, int options)
{
	pid_t ret;

	do {
		ret = waitpid(pid, status, options);
	} while (ret == -1 && errno == EINTR);

	return ret;
}

int fclose_no_EINTR(FILE *fp)
{
	int ret;

	do {
		ret = fclose(fp);
	} while (ret == -1 && errno == EINTR);

	return ret;
}
