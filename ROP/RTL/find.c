#include <stdio.h>
#include <string.h>

int main()
{
    long *system = 0xf7e0f620;

    while (memcmp(system, "/bin/sh\x00", 8))
        system++;
    printf("/bin/sh: %p\n", system);

}