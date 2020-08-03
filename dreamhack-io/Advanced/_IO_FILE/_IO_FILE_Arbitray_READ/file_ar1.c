// gcc -o file_ar1 file_ar1.c
#include <stdio.h>
#include <string.h>
int main()
{
	char *buf = "THIS IS TEST FILE!\0";
	FILE *fp;
	fp = fopen("testfile","w"); 
	fwrite(buf, 1, strlen(buf), fp);
	return 0;
}
