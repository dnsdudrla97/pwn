// gcc -o tcache_ex tcache_ex.c -no-pie
#include <stdio.h>
#include <stdlib.h>
int main()
{
	char *ptr = malloc(0x10);
	char *ptr2 = malloc(0x20);
	char *ptr3 = malloc(0x30);
	free(ptr);
	free(ptr2);
	free(ptr3);
	ptr = malloc(0x10);
	ptr2 = malloc(0x20);
	ptr3 = malloc(0x30);
}