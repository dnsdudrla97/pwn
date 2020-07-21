// gcc -o f_error1 f_error1.c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
int main()
{
    uint64_t *ptr;
	// p > -size
    ptr = malloc(0x100);
	ptr[-1] = -0x100;
	free(ptr);	
}