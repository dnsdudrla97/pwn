//gcc -o example4 example4.c -fno-stack-protector -mpreferred-stack-boundary=2 -m32 -no-pie
#include <stdio.h>
int main(void){
  char buf[32] = {};
  puts("Hello World!");
  puts("Hello ASLR!");
  scanf("%s", buf);
  return 0;
}
