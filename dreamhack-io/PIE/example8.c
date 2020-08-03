// gcc -o example8 example8.c -m32 -fno-stack-protector -mpreferred-stack-boundary=2 -fPIC -pie
#include <stdio.h>
void give_shell(void){
  system("/bin/sh");
}
void vuln(void){
  char buf[32] = {};
  printf("Input1 > ");
  read(0, buf, 512);    // Buffer Overflow
  printf(buf);          // Format String Bug
  printf("Input2 > ");
  read(0, buf, 512);    // Buffer Overflow
}
int main(void){
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  vuln();
}