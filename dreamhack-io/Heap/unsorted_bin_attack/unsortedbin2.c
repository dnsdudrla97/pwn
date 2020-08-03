#include <stdio.h>
#include <stdlib.h>

#define ALLOC_SIZE 0x410
long target;

int main(void){
    fprintf(stderr, "target : 0x%lx\n", target);
    long *ptr = malloc(ALLOC_SIZE);
    malloc(ALLOC_SIZE);
    free(ptr);
    ptr[1] = (long)&target - 16;
    malloc(ALLOC_SIZE);
    fprintf(stderr, "target : 0x%lx\n", target);
}