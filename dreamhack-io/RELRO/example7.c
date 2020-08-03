// gcc -o example7 example7.c -mpreferred-stack-boundary=2 -Wl,-z,relro,-z,now

#include <stdio.h>

void arbitrary_read(long * addr){
  printf("%lx\n", *addr);
}
void arbitrary_write(long *addr, long val){
  *addr = val;
}
void menu(void){
  puts("0. exit");
  puts("1. Leak Address");
  puts("2. Arbitrary Write");
}
int main(void){
  
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  int choice = 0;
  long addr = 0;
  long value = 0;
  
  while(1){
    menu();
    scanf("%d", &choice);
    switch(choice){
      case 1:
        printf("Which address? : ");
        scanf("%lu", &addr);
        
        arbitrary_read(addr);
        break;
      case 2:
        printf("Address : ");
        scanf("%lu", &addr);
        printf("Value : ");
        scanf("%lu", &value);
        
        arbitrary_write(addr, value);
        break;
      
      case 0:
        return 0;
      default:
        break;
    }
  }
}