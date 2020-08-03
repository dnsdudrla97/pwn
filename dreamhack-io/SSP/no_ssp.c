// gcc -o no_ssp no_ssp.c -m32 -fno-stack-protector
#include <stdio.h>
#include <string.h>
void func(char *s){
    char buf[16] = {};
    strcpy(buf, s);
}
int main(int argc, char *argv[]){
	func(argv[1]);
}