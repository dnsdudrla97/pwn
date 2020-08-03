#define MAYBE_INIT_TCACHE() \
  if (__glibc_unlikely (tcache == NULL)) \
    tcache_init();
void * __libc_malloc (size_t bytes)
{
    ...
    MAYBE_INIT_TCACHE ();
}