#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
int main() {
  char *ptr = malloc(0x100);
  char *ptr2 = malloc(0x100);
  free(ptr); // ptr == unsorted bin 
  char *ptr3 = malloc(0x100);
}