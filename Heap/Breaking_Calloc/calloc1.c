// gcc -o calloc1 calloc1.c -no-pie 
#include <stdlib.h>
int main()
{
	char *ptr,*ptr2,*ptr3;
	ptr = calloc(10, sizeof(char));
	ptr2 = calloc(10, sizeof(int));
	ptr3 = calloc(10, sizeof(double));
	printf("ptr: %d", malloc_usable_size(ptr));
	printf("ptr2: %d", malloc_usable_size(ptr2));
	printf("ptr3: %d", malloc_usable_size(ptr3));
}