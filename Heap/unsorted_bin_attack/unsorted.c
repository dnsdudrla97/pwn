// gcc -o unsorted unsorted.c -fno-stack-protector
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char name[16];

int main()
{
	char buf[256];
	char *ptr[10];
	int ch, idx;
	int i = 0;
	setvbuf(stdout, 0, 2, 0);
	setvbuf(stdin, 0, 2, 0);
	while (1) {
		printf("> ");
		scanf("%d", &ch);
		switch(ch) {
			case 1: 
				if( i >= 10 ) {
					printf("Do not overflow\n");
					exit(0);
				} 
				ptr[i] = malloc(0x100);
				printf("Data: ");
				read(0, ptr[i], 0x100);
				i++;
				break;
			case 2:
				printf("idx: ");
				scanf("%d", &idx);
				free(ptr[idx]);
				break;
			case 3:
				printf("idx: ");
				scanf("%d", &idx);
				printf("data: ");
				read(0, ptr[idx], 0x100);
                break;
			case 4:
                printf("Name: %s\n",name);
				break;
			case 5:
				read(0, buf, 300);
				return 0;
			default:
				break;
		}
	}
	return 0;
}