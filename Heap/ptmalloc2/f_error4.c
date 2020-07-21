// gcc -o f_error4 f_error4.c 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
int main()
{
        uint64_t *ptr,*ptr2;
        ptr = malloc(0x10);
        ptr2 = malloc(0x10);
        ptr2[-1] = 0x22001;
        free(ptr);
}