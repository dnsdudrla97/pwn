// gcc -o file1 file1.c 
#include <stdio.h>
#include <unistd.h>
#include <string.h>
int main()
{
	FILE *fp;
	char buf[256] = {0, };
	strcpy(buf, "THIS IS TESTFILE!");
	fp = fopen("testfile","w");
	fwrite(buf, 1, strlen(buf), fp);
	fclose(fp);
	return 0;
}