// gcc -o spirit2 spirit2.c -no-pie
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void giveshell()
{
	system("/bin/sh");
}

int main()
{
	setvbuf(stdin,0,2,0);
	setvbuf(stdout,0,2,0);
	char *ptr[10] = {0,};
	long long idx = 0;
	size_t size = 0;
	long long index = 0;
	size_t address = 0;
	int i = 0;
	size_t *ptr_size[10] = {0,};

	printf("%p\n", &size); // 스택 주소 노출
	while(1) {
		printf("1. Add\n");
		printf("2. Free\n");
		printf("3. Edit\n");
		printf(">");
		scanf("%d",&idx);
		switch(idx) {
// 힙을 10번 할당할 수 있다. 
// 이후에 3번 메뉴에서 값을 쓰기 위해 ptr과 ptr_size 변수에 힙 포인터와 할당된 크기를 대입한다..
			case 1:
				if( i >= 10 ) {
					break;
				}
				printf("Size: ");
				scanf("%llu",&size);
				ptr[i] = malloc(size);
				ptr_size[i] = size;
				i++;
				break;
// 원하는 주소를 해제할 수 있다.
			case 2:
				printf("Address: ");
				scanf("%lld", &address);
				free(address);
				break;
// 할당한 힙 영역에 데이터를 입력할 수 있다.
			case 3:
				printf("Index: ");
				scanf("%llu", &index);
				read(0, ptr[index], ptr_size[index]);
				break;
// 1, 2, 3번 메뉴가 아니라면 프로그램을 종료한다.
			default:
				return 0;
		}
	}
	return 0;
}