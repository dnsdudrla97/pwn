// gcc -o f_error3 f_error3.c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
int main()
{
    uint64_t *ptr;
	// size < MINSIZE
    ptr = malloc(0x10);
	ptr[-1] = 0x19;
	free(ptr);
}
