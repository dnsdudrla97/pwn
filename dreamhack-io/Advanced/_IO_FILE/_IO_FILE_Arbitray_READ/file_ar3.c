// gcc -o file_ar3 file_ar3.c -no-pie
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
char dummy;
char flag_buf[128];
int read_flag() {
	FILE *fp;
	fp = fopen("flag", "r");
	fread(flag_buf, 1, 256, fp);
	fclose(fp);
}
int main()
{
	FILE *fp;
	long long addr = 0; 
	long long value = 0;
	int bytes;
	char *data = "TEST file\0";
	
	read_flag();
	fp = fopen("testfile", "w");
	
	printf("FILE PTR: %p\n", fp);
	fflush(stdout);
	
	printf("Addr: ");
	fflush(stdout);
	scanf("%ld", &addr);
	printf("Value: ");
	fflush(stdout);
	read(0, addr, 200);
		
	fwrite(data, 1, strlen(data), fp);
	fclose(fp);
	return 0;
}