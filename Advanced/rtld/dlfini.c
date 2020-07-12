// gcc -o dlfini dlfini.c -no-pie -z relro -z now
#include <stdio.h>
#include <stdlib.h>
int main()
{
	long long addr;
	long long data;

    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    
	printf("stdout: %lp\n",stdout);

	printf("addr: ");
	scanf("%ld",&addr);

	printf("data: ");
	scanf("%ld",&data);

	*(long long *)addr = data;
    
	exit(0);
}