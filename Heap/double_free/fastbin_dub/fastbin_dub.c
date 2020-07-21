#include <stdio.h>
#include <stdlib.h>
int main(void){
  char *ptr1 = (char *)malloc(0x40);
  char *ptr2 = (char *)malloc(0x40);
  free(ptr1);
  free(ptr2);
  free(ptr1);
  fprintf(stderr, "malloc : %p\n", malloc(0x40));
  fprintf(stderr, "malloc : %p\n", malloc(0x40));
  fprintf(stderr, "malloc : %p\n", malloc(0x40));
  return 0;
  
}