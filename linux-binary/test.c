#include <stdio.h>
#include <stdlib.h>

static inline void func1() {}
void func2() {}

int main(char argc, char** argv) {

	unsigned char n;
	printf("%s\n",argv[1]);

	func2();
	func1();
	return 0;

}

