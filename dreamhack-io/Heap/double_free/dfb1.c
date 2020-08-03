// gcc -o dfb1 dfb1.c
#include <stdlib.h>
int main()
{
	char ptr = malloc(32);
	char ptr2 = malloc(32);
	
	free(ptr);
	free(ptr);
	return 0;
}