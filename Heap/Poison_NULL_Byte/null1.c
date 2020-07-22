// gcc -o null1 null1.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int readdata(char *buf, int len){
        read(0, buf, len);
        *(char *)(buf + len) = '\x00';
}
int main(){
        char* buf1;
        char* buf2;
        buf1 = malloc(0x28);
        buf2 = malloc(0xf0);
        readdata(buf1, 0x28);
        return 0;
}