#define chunk_is_mmapped(p) ((p)->mchunk_size & IS_MMAPPED)
p = mem2chunk (mem);
  /* Two optional cases in which clearing not necessary */
  if (chunk_is_mmapped (p))
    {
      if (__builtin_expect (perturb_byte, 0))
        return memset (mem, 0, sz);
      return mem;
    }