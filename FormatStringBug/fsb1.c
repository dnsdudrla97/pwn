// gcc -o fsb1 fsb1.c -m32 -mpreferred-stack-boundary=2
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char flag_buf[50];

void initialize() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
}

int main(void)
{
	FILE *fp;
	char buf[256];

	initialize();

	memset(buf, 0, sizeof(buf));

	fp = fopen("./flag", "r");

	fread(flag_buf, 1, sizeof(flag_buf), fp);

	printf("Input: ");

	read(0, buf, sizeof(buf)-1);

	printf(buf);

	return 0;
}



