// gcc -o gadget gadget.c 
void gadget() {
	asm("pop %rdi");
	asm("pop %rsi");
	asm("pop %rdx");
	asm("pop %rax");
	asm("ret");
}
void gadget2() {
	asm("syscall");
}
int main()
{
	return 0;
}