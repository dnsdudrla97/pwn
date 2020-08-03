#include <stdio.h>
#include <stdlib.h>
int main()
{
	char *ptr = malloc(256);
	char *ptr2 = malloc(256);
	free(ptr);
	free(ptr2);
	return 0;
}