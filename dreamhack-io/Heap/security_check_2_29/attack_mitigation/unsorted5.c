// gcc -o unsorted5 unsorted5.c
#include <stdlib.h>
#include <stdint.h>
int main()
{
	uint64_t *unsorted_bin = malloc(0x430);
	uint64_t *ptr = malloc(0x430);
	free(unsorted_bin);
	ptr[-1] = 0x111; // ptr size field
	uint64_t *error_ptr = malloc(0x430);
}