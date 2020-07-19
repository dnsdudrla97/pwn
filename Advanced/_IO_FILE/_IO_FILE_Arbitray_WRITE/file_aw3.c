// gcc -o file_aw3 file_aw3.c --no-pie 
#include <string.h>
#include <stdio.h>
#include <unistd.h>
char flag_buf[128];
int overwrite_me = 0;
int read_flag() {
	FILE *fp;
	fp = fopen("flag", "r");
	fread(flag_buf, 1, 256, fp);
	printf("FLAG: %s\n", flag_buf);
	fclose(fp);
}
int main()
{
	FILE *fp;
	long long addr = 0; 
	long long value = 0;
	char buf[10];
	fp = fopen("testfile", "r");
	
	printf("FILE PTR: %p\n", fp);
	fflush(stdout);
	
	printf("Addr: ");
	fflush(stdout);
	scanf("%ld", &addr);
	printf("Value: ");
	fflush(stdout);
	read(0, addr, 200);
		
	fread(buf, 1, strlen(buf)-1, fp);
	if( overwrite_me == 0xDEADBEEF ) {
		read_flag();
	}
	fclose(fp);
	return 0;
}