// gcc -o file_aw2 file_aw2.c 
#include <stdio.h>
int main()
{	
	char overwrite_me[256];
	char buf[256];
	FILE *fp;
	fp = fopen("testfile","r");
	
	fp->_IO_buf_base = overwrite_me;
	fp->_IO_buf_end = overwrite_me + 256;
	fp->_fileno = 0;
	
	fread(buf, 1, 5, fp);
	printf("overflow_me: %s\n",overwrite_me);
	return 0;
}