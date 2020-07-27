#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
char flag_buf[30];
void read_flag() {
	FILE *fp;
	fp = fopen("./flag","r");
	fread(flag_buf,1, sizeof(flag_buf)-1, fp);
	fclose(fp);
}
int main()
{
	setvbuf(stdin,0,2,0);
	setvbuf(stdout,0,2,0);
	char *ptr[20] = {0,};
	long long idx = 0;
	size_t size = 0;
	long long index = 0;
	int i = 0;
	read_flag();
	while(1) {
		printf("1. Add\n");
		printf("2. Free\n");
		printf("3. Edit\n");
		printf("4. Show\n");
		printf(">");
		scanf("%d",&idx);
		switch(idx) {
			case 1:
				if( i >= 20 ) {
					break;
				}
				printf("Size: ");
				scanf("%llu",&size);
				ptr[i] = calloc(size,sizeof(char));
				i++;
				break;
			case 2:
				printf("Index: ");
				scanf("%lld",&index);
				memcpy(ptr[index],flag_buf, strlen(flag_buf));
				free(ptr[index]);
				ptr[index] = 0;
				break;
			case 3:
				printf("Index: ");
				scanf("%llu", &index);
				if( index >= 20 ) {
					break;
				} 
				read(0, ptr[index], 30);
				break;
			case 4:
				printf("Index: ");
				scanf("%llu", &index);
				if( index >= 20 ) {
					break;
				}
				printf("ptr: %s\n",ptr[index]);
				break;
			default:
				return 0;
		}
	}
	return 0;
}