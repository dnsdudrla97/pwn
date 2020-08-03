// gcc -o master2 master2.c -pthread
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void giveshell()
{
    execve("/bin/sh", 0, 0);
}

int thread_routine()
{
    char buf[256];
    int size = 0;
    printf("Size: ");
    scanf("%d", &size);
    printf("Data: ");
    read(0, buf, size);
    return 0;
}

int main()
{
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    pthread_t thread_t;
    if (pthread_create(&thread_t, NULL, thread_routine, NULL) < 0)
    {
        perror("thread create error:");
        exit(0);
    }
    pthread_join(thread_t, 0);
    return 0;
}


0x926b66425919f800

0x0000000000400a05 in main ()
gdb-peda$ find 0x926b66425919f800
Searching for '0x926b66425919f800' in: None ranges
Found 1 results, display max 1 items:
mapped : 0x7f6590591728 --> 0x926b66425919f800 

Searching for '0x926b66425919f800' in: None ranges
Found 4 results, display max 4 items:
 mapped : 0x7f658fd8df48 --> 0x926b66425919f800 
 mapped : 0x7f658fd8e728 --> 0x926b66425919f800 
 mapped : 0x7f6590591728 --> 0x926b66425919f800 
[stack] : 0x7ffc8df74f98 --> 0x926b66425919f800 
gdb-peda$ 

0x7f658fd8de40-0x7f6590591728

0x7f658fd8e728

0x4009f3


ret 0x0000000000400aa5

TLS:1


mapped : 0x7f2561b07728 --> 0x8e5ceda739875b00 


Searching for '0x8e5ceda739875b00' in: None ranges
Found 3 results, display max 3 items:

 mapped : 0x7f2561303f48 --> 0x8e5ceda739875b00 # TLS : 3 thread_routine()_fini
 mapped : 0x7f2561304728 --> 0x8e5ceda739875b00 # TLS : 2 thread_routine()_init
 mapped : 0x7f2561b07728 --> 0x8e5ceda739875b00 # TLS : 1 main
[stack] : 0x7fff7a4c1b08 --> 0x8e5ceda739875b00 # TLS : 0 stack

thread_routine() rsp -> ret 주소 까지 차이 : 0x118


p/x 0x7fff7a4c1b18-0x7f2561303f58

ret = 0x7fff7a4c1b18