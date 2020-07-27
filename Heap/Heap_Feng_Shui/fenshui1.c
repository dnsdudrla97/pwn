#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int add();
int del();
int edit();
int show();
char *ptr[20];
int ptr_size[20];
int heap_idx = 0;

int main()
{
	int idx;
	setvbuf(stdout, 0, 2, 0);
	setvbuf(stdin, 0, 2, 0);
	while(1) {
		printf("1. Add\n");
		printf("2. Del\n");
		printf("3. Edit\n");
		printf("4. Show\n");
		printf("> ");
		scanf("%d",&idx);
		switch(idx) {
			case 1:
				add();
				break;
			case 2:
				del();
				break;
			case 3:
				edit();
				break;
			case 4:
				show();
				break;
			default:
				printf("Bye\n");
				exit(0);
		}
	}
	return 0;
}

int add() {
	int size;
	if(heap_idx >= 20 ) {
		exit(0);
	}
	printf("Size: ");
	scanf("%d",&size);
	ptr_size[heap_idx] = size;
	ptr[heap_idx] = (char *)malloc(size);
	heap_idx++;
	return 0;
}
int del() {
	int idx;
	printf("idx: ");
	scanf("%d",&idx);
	if(idx >= 20) {
		exit(0);
	}
	free(ptr[idx]);
	ptr[idx] = 0;
	return 0;
}
int edit() {
	int idx;
	printf("idx: ");
	scanf("%d", &idx);
	if(idx >= 20) {
		exit(0);
	}
	printf("Data: ");
	read(0, ptr[idx], ptr_size[idx]+100);
	return 0;
}
int show() {
	int idx;
	printf("idx: ");
	scanf("%d", &idx);
	if(idx >= 20) {
		exit(0);
	}
	printf("Data: %s", ptr[idx]);
	return 0;
}