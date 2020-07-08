#include <stdio.h>
int main(void){
  char buf[32] = {};
  puts("Hello World!");
  puts("Hello ASLR!");
  scanf("%s", buf);
  return 0;
}
