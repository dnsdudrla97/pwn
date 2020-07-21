#include <stdlib.h>
int main()
{
	char *ptr = malloc(32);
	char *ptr2 = malloc(32);
	char *ptr3 = malloc(32);
	
	free(ptr);
	free(ptr2);
	free(ptr3);
	return 0;
}