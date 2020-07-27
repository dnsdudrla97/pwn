// gcc -o spirit1 spirit1.c -no-pie
#include <stdio.h>
#include <stdlib.h>
int main()
{
	long long fake_chunk[10] = {0,};
	fake_chunk[0] = 0;
	fake_chunk[1] = 0x31;
	fake_chunk[2] = 0x41414141;

	free(&fake_chunk[2]);
	char *fake_alloc = malloc(0x20);
	printf("fake chunk: %p\n", fake_alloc);
}