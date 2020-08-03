// gcc -o lore2 lore2.c -fno-stack-protector
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *ptr[20];
int ptr_size[20];
int heap_idx = 0;
int add() {
	int size;
	if( heap_idx > 20 ) {
		exit(0);
	}
	printf("Size: ");
	scanf("%d", &size);
	ptr[heap_idx] = (char *)malloc(size);
	ptr_size[heap_idx] = size;
	printf("Data: ");
	read(0, ptr[heap_idx], size);
	heap_idx++;
	return 0;
}
int del() {
	int idx;
	printf("idx: ");
	scanf("%d", &idx);
	if(idx > 20 ) {
		exit(0);
	}
	free(ptr[idx]);
	return 0;
}
int show() {
	int idx;
	printf("idx: ");
	scanf("%d", &idx);
	printf("%p: %s\n", ptr[idx], ptr[idx]);
	return 0;
}
int edit() {
	int idx;
	printf("idx: ");
	scanf("%d", &idx);
	printf("Data: ");
	read(0, ptr[idx], ptr_size[idx]);
	return 0;
}
int main() {
	char name[100];
	int idx;
	setvbuf(stdin, 0, 2, 0);
	setvbuf(stdout, 0, 2, 0);
	printf("name: ");
	read(0, name, sizeof(name)-1);
	printf("%p: %s",&name, name);
	while(1) { 
		printf("1. Add\n");
		printf("2. Delete\n");
		printf("3. Show\n");
		printf("4. Edit\n");
		printf("5. Edit name\n");
		printf("> ");
		scanf("%d", &idx);
		switch(idx) {
			case 1:
				add();
				break;
			case 2:
				del();
				break;
			case 3:
				show();
				break;
			case 4:
				edit();
				break;
			case 5:
				printf("Name: ");
				read(0, name, sizeof(name)-1);
				break;
			default:
				return 0;
		}
	}
	return 0;
}