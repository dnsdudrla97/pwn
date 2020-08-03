// gcc -o master1 master1.c
#include <stdio.h>
#include <unistd.h>
int main()
{
	char buf[256];
	read(0, buf, 256);
}