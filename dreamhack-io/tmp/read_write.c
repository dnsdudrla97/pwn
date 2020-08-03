//gcc -o read_write read_write.c -m32

//read_write.c

#include <stdio.h>

int main(void){

  char buf[64] = {};

  printf("Input : ");

  scanf("%63s", buf);

  printf("Your input : %s", buf);

  

}
