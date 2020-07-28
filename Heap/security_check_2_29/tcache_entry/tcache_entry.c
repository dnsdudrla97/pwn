// GLIBC 2.26
typedef struct tcache_entry
{
  struct tcache_entry *next;
} tcache_entry;

// GLIBC 2.29
typedef struct tcache_entry
{
  struct tcache_entry *next;
  /* This field exists to detect double frees.  */
  struct tcache_perthread_struct *key;
} tcache_entry;