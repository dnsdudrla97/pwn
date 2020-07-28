// gcc -o unsorted3 unsorted3.c
#include <stdlib.h>
#include <stdint.h>
int main()
{
	uint64_t *unsorted_bin = malloc(0x430);
	uint64_t *ptr = malloc(0x430);
	free(unsorted_bin);
	ptr[-2] = 0x1234; // ptr prev_size = 0x1234
	uint64_t *error_ptr = malloc(0x430);
}