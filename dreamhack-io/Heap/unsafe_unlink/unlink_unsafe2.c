// gcc -o unlink2 unlink2.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
char *ptr[10];

void getshell()
{
    system("/bin/sh");
}
int main()
{
    int ch, idx, size;
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
                printf("size: ");
                scanf("%d", &size);
                printf("data: ");
                read(0, ptr[idx], size);
                break;
            case 4:
                exit(0);
                break;
            default:
                break;
        }
    }
    return 0;
}