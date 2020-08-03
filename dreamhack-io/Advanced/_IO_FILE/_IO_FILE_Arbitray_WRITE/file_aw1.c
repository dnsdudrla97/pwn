// gcc -o file_aw1 file_aw1.c
#include <stdio.h>
int main()
{	
	char buf[256];
	FILE *fp;
	fp = fopen("testfile","r");
	fread(buf, 1, 256, fp);
	printf("%s\n",buf);
	return 0;
}