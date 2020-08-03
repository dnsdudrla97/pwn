// gcc -o hook2 hook2.c
#include <stdio.h>
#include <malloc.h>

int main()
{
        long int *ptr;
        printf("stdout@LIBC addr: %p\n", *&stdout);
        for(int i=0; i < 10; i++) {
                ptr = (long int *)malloc(1024);              
                read(0, ptr, 1024-1);
                
                *(long int *)*ptr = *(long int *)(ptr+1);
                free(ptr);
        }
}