// gcc m_error2 m_error2.c 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
int main()
{
        uint64_t *ptr,*ptr2,*ptr3;
        ptr = malloc(0x100);
        ptr2 = malloc(0x100);
        free(ptr);
        fprintf(stderr, "BK: %p\n", ptr[1]);
        ptr[1] += 0x40;
        fprintf(stderr, "Corrupted BK: %p\n", ptr[1]);
        malloc(0x21000);
}