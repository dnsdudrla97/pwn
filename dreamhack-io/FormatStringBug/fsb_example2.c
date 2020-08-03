#include <stdio.h>

int main()
{
    int ret = 0;
    printf("1234%n\n", &ret);
    printf("ret : %d\n", ret);
}