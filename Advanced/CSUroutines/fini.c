// gcc -o fini fini.c -z norelro
#include <stdio.h>
#include <stdlib.h>

void getshell() {
	system("/bin/sh");
}

int main()
{
	long int overwrite_addr = 0;
	long int overwrite_value = 0;
	printf("Overwrite addr: ");
	scanf("%ld", &overwrite_addr);
	printf("Value: ");
	scanf("%ld",&overwrite_value);
	
	*(long int*)overwrite_addr  = overwrite_value;
	return 0;
}