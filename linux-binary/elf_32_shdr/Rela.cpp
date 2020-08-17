typedef struct
{
    Elf64_Addr r_offset;
    uint64_t r_info;
    int64_t r_addend;
} Elf64_Rela;