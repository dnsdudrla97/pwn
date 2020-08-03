//gcc -o ssp_fork1 ssp_fork1.c -m32 -fstack-protector-all -no-pie
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
        pid_t pid;
        pid = fork(); //fork
        if(pid == -1) { //fork error
                printf("error\n");
        }
        if(pid == 0) { // child
                printf("Im child!\n");
                printf("Process ID : %d\n", getpid());
        }
        else{ //parent
                printf("Im parent!\n");
                printf("Process ID : %d\n", getpid());
        }
}