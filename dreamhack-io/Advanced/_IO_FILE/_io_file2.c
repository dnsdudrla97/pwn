// gcc -o file2 file2.c 
#include <stdio.h>
int main()
{
	char file_data[256];
	int ret;
	FILE *fp;
	
	strcpy(file_data, "AAAA");
	fp = fopen("testfile","r");
	fread(file_data, 1, 256, fp);
	printf("%s",file_data);
	fclose(fp);
}