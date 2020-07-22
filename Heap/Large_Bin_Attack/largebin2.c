// gcc -o largebin2 largebin2.c -no-pie
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
typedef struct counter {
	char name[0x20];
	int count;
	void (*func)(void *);
} counter;
void print_count(counter *c) {
	printf("%s: %d\n", c->name, c->count);
}
void giveshell() {
	system("/bin/sh");
}
counter *c;
int main() {
	setvbuf(stdout, 0, 2, 0);
	setvbuf(stdin, 0, 2, 0);
	// [C]
	c = (counter *)malloc(sizeof(counter));
	strcpy(c->name, "Loop Count: ");
	c->count = 0;
	c->func = print_count;
	char *ptr[20] = {0,};
	int i = 0;
	int size = 0;
	int index;
	int select;
	while(1){
		printf("1. Add\n");
		printf("2. Free\n");
		printf("3. Edit\n");
		printf("4. Loop Count\n");
		printf(">");
		scanf("%d",&select);
		switch(select) {
			case 1:
				if( i >= 20 ) {
					break;
				}
				printf("Size: ");
				scanf("%d",&size);
				if(size < 0x100) {
					printf("small\n");
					break;
				}
				ptr[i] = malloc(size);
				printf("alloc\n");
				i++;
				break;
			case 2:
				printf("Index: ");
				scanf("%d", &index);
				free(ptr[index]);
				break;
			case 3:
				printf("Index: ");
				scanf("%d", &index);
				printf("Data: ");
				scanf("%s",ptr[index]);
				break;
			case 4:
				c->func(c);
				break;
			default:
				return 0;
		}
		c->count++;
	}
}