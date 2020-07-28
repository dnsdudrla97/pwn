// gcc -o stdout_leak stdout_leak.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
char *ptr[20];
int heap_idx = 0;
int add() {
	int size = 0;
	if(heap_idx >= 20 ) {
		exit(0);
	}
	printf("Size: ");
	scanf("%d", &size);
	ptr[heap_idx] = malloc(size);
	if(!ptr[heap_idx]) {
		exit(0);
	}
	printf("Data: ");
	read(0, ptr[heap_idx], size);
	heap_idx++;
}
int del() {
	int idx;
	printf("idx: ");
	scanf("%d", &idx);
	if(idx >= 20 ) {
		exit(0);
	}
	free(ptr[idx]);
}
int main()
{
	int idx;
	setvbuf(stdin, 0, 2, 0);
	setvbuf(stdout, 0, 2, 0);
	while(1) {
		printf("1. Add\n");
		printf("2. Delete\n");
		printf("> ");
		scanf("%d", &idx);
		switch(idx) {
			case 1:
				add();
				break;
			case 2:
				del();
				break;
			default:
				break;
		}
	}
}