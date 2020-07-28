// gcc -o dfb dfb.c
#include <stdlib.h>
int main()
{
	char *ptr = malloc(256);
	free(ptr);
	free(ptr);
}