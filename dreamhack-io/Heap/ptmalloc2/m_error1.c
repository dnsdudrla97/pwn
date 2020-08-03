// gcc -o m_error1 m_error1.c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
int main()
{
	uint64_t *ptr,*ptr2;
	ptr = malloc(0x20);
	ptr2 = malloc(0x20);
	free(ptr);
	free(ptr2);
	
	fprintf(stderr,"ptr2 fd: %p: %p\n", ptr2, ptr2);
	*(uint64_t *)ptr2 = *(uint64_t *)ptr2 + 0x100;
	fprintf(stderr,"ptr2 fd: %p: %p\n", ptr2, ptr2);		
	malloc(0x20);
	malloc(0x20);
}