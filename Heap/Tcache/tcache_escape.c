// gcc -o tcache_escape tcache_escape.c -no-pie
#include <stdio.h>
#include <stdlib.h>
int main()
{
	char *ptr[8];
	int i;
	for(i=0;i<=7;i++) {
		printf("malloc !\n");
		ptr[i] = malloc(0x20);
	}
	for(i=0;i<=7;i++) {
		printf("free!\n");
		free(ptr[i]);
	}
}