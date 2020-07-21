// gcc -o unlink1 unlink1.c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
int main()
{
    uint64_t *ptr,*ptr2;
    ptr = malloc(0x100);
	ptr2 = malloc(0x100);
	free(ptr);
	free(ptr2);
}