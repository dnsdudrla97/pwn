#include <stdio.h>

int main(void)
{
    char buf[100];

    read(0, buf, 256);
    write(1, buf, 100);

    return 0;
}