typedef struct
{
    uint32_t p_type;        // 세그먼트 형식
    Elf32_Off p_offset;     // 세그먼트 오프셋
    Elf32_Addr p_vaddr;     // 세그먼트 가상 주소 (Segment Virtual Address)
    Elf32_Addr p_paddr;     // 세그먼트 물리 주소 (Segment Physucal Address)
    uint32_t p_filesz;      // 파일에서의 세그먼트 크기
    uint32_t p_memsz;       // 메모리에서의 세그먼트 크기
    uint32_t p_flags;       // 세그먼트 플래그 r,w,x
    uint32_t p_align;       // 메모리에서의 세그먼트 정렬 (Segment Alignment)
} Elf32_Phdr;