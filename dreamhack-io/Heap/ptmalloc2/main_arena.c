// gcc -o main_arena main_arena.c
#include <stdio.h>
#include <malloc.h>
int main()
{
	char *ptr = malloc(0x10);
	free(ptr);
	char *ptr2 = malloc(0x20);
	free(ptr2);
	
}
