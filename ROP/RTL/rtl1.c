// sysctl -w kernel.randomize_va_space=0

#include <stdio.h>
#include <unistd.h>

int main()
{
    char buf[256];
    read(0, buf, 512);
    printf("%s", buf);
}
