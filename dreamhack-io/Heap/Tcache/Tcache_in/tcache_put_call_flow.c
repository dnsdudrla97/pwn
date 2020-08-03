
# define TCACHE_MAX_BINS		64
# define csize2tidx(x) (((x) - MINSIZE + MALLOC_ALIGNMENT - 1) / MALLOC_ALIGNMENT)
#if USE_TCACHE
size_t tc_idx = csize2tidx (nb);
if (tcache && tc_idx < mp_.tcache_bins)         
{
	mchunkptr tc_victim;
	/* While bin not empty and tcache not full, copy chunks over.  */
	while (tcache->counts[tc_idx] < mp_.tcache_count && (pp = *fb) != NULL)
	{
		REMOVE_FB (fb, tc_victim, pp);
		if (tc_victim != 0)
		{
			tcache_put (tc_victim, tc_idx);
        }
}