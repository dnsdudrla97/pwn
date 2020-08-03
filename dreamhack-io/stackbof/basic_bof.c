#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("usage: %s <string>\n", argv[0]);
		exit(0);
	}
	char buf[256];
	strcpy(buf, argv[1]);

	printf("buf : %s\n", buf);
	return 0;
}
