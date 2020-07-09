// gcc -o asm asm.c -zexecstack
#include <stdio.h>
#include <unistd.h>
int main() {
	char shellcode[256];
	
	read(0, shellcode, 256);
	int (*shell)() = (int (*)())shellcode;
	
	shell();
}