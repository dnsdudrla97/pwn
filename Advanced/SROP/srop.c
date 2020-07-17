// gcc -o srop srop.c -fno-stack-protector
#include <stdio.h>
int gadget() {
	__asm("pop %rax");
	__asm("syscall");
	__asm("ret");
}
int main()
{
	char buf[16];
	read(0, buf ,1024);
}