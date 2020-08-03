//  2038  sysctl -w kernel.randomize_va_space=0
#include <stdio.h>

int main()
{
    char buf[20];

    gets(buf);
    puts(buf);
    return 0;
}