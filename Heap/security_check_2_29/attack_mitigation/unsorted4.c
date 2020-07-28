// gcc -o unsorted4 unsorted4.c
#include <stdlib.h>
#include <stdint.h>
uint64_t target;
int main()
{
	uint64_t *unsorted_bin = malloc(0x430);
	uint64_t *ptr = malloc(0x430);
	free(unsorted_bin);
	unsorted_bin[1] = &target; // unsorted_bin->bk = &target
	uint64_t *error_ptr = malloc(0x430);
}