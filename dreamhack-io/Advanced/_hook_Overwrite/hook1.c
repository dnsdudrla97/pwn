// gcc -o hook1 hook1.c
#include <stdio.h>
#include <malloc.h>
int main()
{
	__malloc_hook = 0x41414141;
	malloc(1);
}