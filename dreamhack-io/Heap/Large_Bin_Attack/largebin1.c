// gcc -o largebin1 largebin1.c 
#include<stdio.h>
#include<stdlib.h>
 
int main()
{
    unsigned long var1 = 0, var2 = 0;
    unsigned long *p1, *p2, *p3;
    p1 = malloc(0x300); malloc(0x100);
    p2 = malloc(0x400); malloc(0x100);
    p3 = malloc(0x410); malloc(0x100);
    free(p1);
    free(p2);
    malloc(0x100);
    free(p3);
    p2[1] = (unsigned long)(&var1 - 2);
    p2[3] = (unsigned long)(&var2 - 4);
    fprintf(stderr, "var1(%p): %p\n", &var1, (void *)var1);
    fprintf(stderr, "var2(%p): %p\n", &var2, (void *)var2);
    malloc(0x90);
    fprintf(stderr, "var1(%p): %p\n", &var1, (void *)var1);
    fprintf(stderr, "var2(%p): %p\n", &var2, (void *)var2);
    return 0;
}