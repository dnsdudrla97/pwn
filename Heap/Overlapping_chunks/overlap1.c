// gcc -o overlap1 overlap1.c 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
int main(int argc , char* argv[]){
	uint64_t *ptr1, *ptr2, *ptr3;
	uint64_t over_ptr; 
	ptr1 = malloc(0x100);
	ptr2 = malloc(0x100);
	ptr3 = malloc(0x100);
	free(ptr1);
	ptr1[-1] = 0x211;          
	over_ptr = malloc(0x200);
	memset(over_ptr, 0x41, 0x200);
}