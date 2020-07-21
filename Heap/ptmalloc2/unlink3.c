// gcc -o unlink3 unlink3.c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
int main()
{
    uint64_t *ptr,*ptr2;
    ptr = malloc(0x1000);
	ptr2 = malloc(0x1000);
	free(ptr);
	ptr[2] = ptr+0x100;
	ptr[3] = ptr+0x100;
	free(ptr2);
}