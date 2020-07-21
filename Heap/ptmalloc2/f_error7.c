// gcc f_error7 f_error7.c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
int main()
{
    uint64_t *ptr,*ptr2;
    ptr = malloc(0x100);
	ptr2 = malloc(0x100);
	ptr2[-1] = 0x21001;
	free(ptr2);
}
