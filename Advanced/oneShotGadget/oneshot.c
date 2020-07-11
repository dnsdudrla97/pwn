// gcc -o oneshot oneshot.c -fno-stack-protector
#include <stdio.h>
#include <unistd.h>
int main()
{
    char buf[16];
    printf("stdout: %p\n", stdout);
    fflush(stdout);
    printf("Input ME: ");
    fflush(stdout);
    read(0, buf ,32);
    return 0;
}