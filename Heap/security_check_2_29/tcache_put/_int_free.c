tcache_entry *e = (tcache_entry *) chunk2mem (p);
/* This test succeeds on double free.  However, we don't 100%
   trust it (it also matches random payload data at a 1 in
   2^<size_t> chance), so verify it's not an unlikely
   coincidence before aborting.  */
if (__glibc_unlikely (e->key == tcache))
{
	tcache_entry *tmp;
	LIBC_PROBE (memory_tcache_double_free, 2, e, tc_idx);
	for (tmp = tcache->entries[tc_idx]; tmp; tmp = tmp->next)
		if (tmp == e)
			malloc_printerr ("free(): double free detected in tcache 2");
			/* If we get here, it was a coincidence.  We've wasted a
			   few cycles, but don't abort.  */
}