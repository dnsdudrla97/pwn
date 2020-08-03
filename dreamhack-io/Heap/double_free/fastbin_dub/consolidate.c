// gcc -o consolidate consolidate.c
#include <stdio.h>
#include <stdlib.h>
int main() {
  void* p1 = malloc(0x10);
  void* p2 = malloc(0x10);
  free(p1);
  void* p3 = malloc(0x400);
  free(p1);
  char *ptr = malloc(0x10);
  char *ptr2 = malloc(0x10);
  printf("ptr: %p\n", ptr);
  printf("ptr2: %p\n", ptr2);
}
