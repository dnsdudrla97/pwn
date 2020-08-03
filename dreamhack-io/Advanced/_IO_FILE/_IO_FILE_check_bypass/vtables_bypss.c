// gcc -o vtable_bypass vtable_bypass.c -no-pie 
#include <stdio.h>
#include <unistd.h>

FILE *fp;

int main() {
	setvbuf(stdin, 0, 2, 0);
	setvbuf(stdout, 0, 2, 0);
	fp = fopen("/dev/urandom","r");
	printf("stdout: %p\n",stdout);
	printf("Data: ");
	read(0, fp, 300);
	fclose(fp);
}