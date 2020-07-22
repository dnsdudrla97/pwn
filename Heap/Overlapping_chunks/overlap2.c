// gcc -o overlap2 overlap2.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(int argc , char* argv[]){
	uint64_t *ptr1, *ptr2, *ptr3;
	uint64_t over_ptr; 

	ptr1 = malloc(0x20);
	ptr2 = malloc(0x20);
	ptr3 = malloc(0x20);
    
	ptr1[-1] = 0x61;
	free(ptr1);
	over_ptr = malloc(0x50);
	memset(over_ptr, 0x41, 0x50);
}