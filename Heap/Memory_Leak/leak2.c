// gcc -o leak2 leak2.c -fno-stack-protector
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    char buf[256];
    char *ptr[10];
    int ch, idx;
    int i = 0;

    setvbuf(stdout, 0, 2, 0);
    setvbuf(stdin, 0, 2, 0);

    while (1)
    {
        printf("> ");
        scanf("%d", &ch);
        switch (ch)
        {
        // 0x100 바이트 크기의 힙을 할당할 수 있고 입력받을 수 있다.
        case 1:
            if (i > 10)
            {
                printf("Do not overflow\n");
                exit(0);
            }
            ptr[i] = malloc(0x100);
            printf("Data: ");
            read(0, ptr[i], 0x100);
            i++;
            break;
        /*
            원하는 힙을 해제할 수 있다.하지만 포인터 초기화가 이루어지지 않기 때문에
            해제된 포인터에 접근이 가능하다.
            */
        case 2:
            printf("idx: ");
            scanf("%d", &idx);
            free(ptr[idx]);
            break;
        /*
            원하는 힙에 데이터를 쓸 수 있다.
            해제 시 포인터 초기화가 이루어지지 않기 때문에 해제된 힙에 접근할 수 있다.
            */
        case 3:
            printf("idx: ");
            scanf("%d", &idx);
            if (i > 10)
            {
                printf("Do not overflow\n");
                exit(0);
            }
            printf("data: ");
            read(0, ptr[idx], 0x100);
            break;
        // 원하는 힙의 데이터를 출력한다.
        case 4:
            printf("idx: ");
            scanf("%d", &idx);
            if (i > 10)
            {
                printf("Do not overflow\n");
                exit(0);
            }
            printf("idx: %d\n", idx);
            printf("data: %s\n", ptr[idx]);
            break;
        /*
            256 바이트 크기의 버퍼에 300 바이트를 입력받음으로 스택 버퍼 오버플로우가 발생한다.
            */
        case 5:
            read(0, buf, 300);
            return 0;
        default:
            break;
        }
    }
    return 0;
}