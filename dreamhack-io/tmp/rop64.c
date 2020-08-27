// gcc -o rop64 rop64.c -fno-stack-protector
#include <stdio.h>
#include <unistd.h>

void gadget() {
	asm("pop %rdi");
	asm("pop %rsi");
	asm("pop %rdx");
	asm("ret");
}

int main()
{
	char buf[256];
	write(1, "Data: ", 6);
	read(0, buf, 1024); 
	return 0;
}