// gcc -o unsorted2 unsorted2.c
#include <stdlib.h>
#include <stdint.h>
int main()
{
	uint64_t *unsorted_bin = malloc(0x430);
	uint64_t *ptr = malloc(0x430);
	free(unsorted_bin);
	// ptr[-1] = 1;    // size < 2 * SIZE_SZ
	ptr[-1] = 0x30000; // size > av->system_mem
	uint64_t *error_ptr = malloc(0x430);
}