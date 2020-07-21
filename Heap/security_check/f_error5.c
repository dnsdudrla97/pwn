// gcc f_error5 f_error5.c 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
int main()
{
    uint64_t *ptr;
    ptr = malloc(0x10);
	free(ptr);
	free(ptr);
}