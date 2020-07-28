// gcc -o unsorted1 unsorted1.c
#include <stdlib.h>
#include <stdint.h>
int main()
{
	uint64_t *unsorted_bin = malloc(0x430);
	uint64_t *ptr = malloc(0x430);
	free(unsorted_bin);
	unsorted_bin[-1] = 1; // size < 2 * SIZE_SZ
	// unsorted_bin[-1] = 0x30000; // size > av->system_mem
	uint64_t *error_ptr = malloc(0x430);
}