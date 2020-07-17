// gcc -o environ environ.c -zexecstack
#include <stdio.h>
#include <unistd.h>
#include <string.h>
long int buf_ptr = 0;
int main()
{
	char buf[1025];
	memset(buf, 0 ,1025);
	read(0, buf, 1024);

	buf_ptr = &buf; 

	long int overwrite_addr;
	long int value;
	
	for(int i=0; i < 10; i++) {
		printf("buf: %s", buf_ptr);
		fflush(stdout);
		printf("Addr: ");
		fflush(stdout);
        
		scanf("%lu", &overwrite_addr);
		printf("Value: ");
		fflush(stdout);
		scanf("%lu", &value);
		
		*(long int *)overwrite_addr = value;
	}
	
	return 0;
}
0x3c6f38