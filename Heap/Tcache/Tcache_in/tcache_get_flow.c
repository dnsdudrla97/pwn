size_t tbytes = request2size (bytes);
size_t tc_idx = csize2tidx (tbytes);
MAYBE_INIT_TCACHE ();
if (tc_idx < mp_.tcache_bins
	&& tcache
	&& tcache->entries[tc_idx] != NULL)
{
	return tcache_get (tc_idx);
}