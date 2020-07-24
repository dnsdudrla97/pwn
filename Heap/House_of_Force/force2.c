// gcc -o force2 force2.c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

__int64_t overwrite_me = 0;

int main(){
	char* buf1;
	char* buf2;
	char* trash;
	char malloc_size[21];

	setvbuf(stdout, 0, 2, 0);
	setvbuf(stdin, 0, 2, 0);

	buf1 = malloc(0x20);
	write(1, &buf1, 8);
	gets(buf1);
	write(1, "input malloc_size : ", 19);
	read(0, malloc_size, 21);
    // 10진수
	trash = malloc(strtoull(malloc_size, NULL, 10));
	buf2 = malloc(0x100);
	write(1, "write to target : ", 17);
	read(0, buf2, 0x100);
	if(overwrite_me == 0xdeadbeefcafebabe){
		system("/bin/sh");
	}
	return 0;
}