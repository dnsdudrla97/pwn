// gcc -o tcache_dfb tcache_dfb.c
#include <stdlib.h>
#include <stdint.h>
int main()
{
	uint64_t *ptr = malloc(0x20);
	free(ptr);
	ptr[1] = 0x0;
	free(ptr);
}