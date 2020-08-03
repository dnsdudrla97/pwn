// gcc -o leak2 leak2.c -no-pie
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
int main()
{
	uint64_t *ptr[2];
	ptr[0] = malloc(0x409);
	ptr[1] = malloc(0x20);
	free(ptr[0]);
	printf("fd: %lp\n", *ptr[0]);
	return 0;
}