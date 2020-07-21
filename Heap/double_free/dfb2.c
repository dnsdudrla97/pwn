// gcc -o dfb2 dfb2.c
#include <stdlib.h>
int main()
{
	char *ptr = malloc(32);     // 0x602010 
	char *ptr2 = malloc(32);    // 0x602030
	
	free(ptr);
    free(ptr2);
	free(ptr);
	return 0;
}
