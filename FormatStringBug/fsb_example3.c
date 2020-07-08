// gcc -o fsb_example3 fsb_example3.c
#include <stdio.h>
int main()
{
	int ret = 0;
	printf("%1024c%1$n\n", &ret);
	printf("ret: %d\n", ret);
}