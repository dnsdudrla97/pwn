// gcc -o ssp ssp.c -m32 -fstack-protector-all
#include <stdio.h>
#include <string.h>
void func(char *s){
    char buf[16] = {};
    /* 
    long canary = stack_guard; 
    */
    strcpy(buf, s);
    /* 
    if (canary != stack_guard)
        stack_chk_fail();
    */
}
int main(int argc, char *argv[]){
	func(argv[1]);
}
