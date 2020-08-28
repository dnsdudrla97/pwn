#include<malloc.h>

int main() {
    __malloc_hook = 0x41414141;
    malloc(1);
}