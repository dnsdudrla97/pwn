if (in_smallbin_range (nb))
{
	idx = smallbin_index (nb);
	bin = bin_at (av, idx);
	if ((victim = last (bin)) != bin)
	{
		if (victim == 0) /* initialization check */
		    malloc_consolidate (av);
		else
	    {
			bck = victim->bk;
			if (__glibc_unlikely (bck->fd != victim))
	        {
				errstr = "malloc(): smallbin double linked list corrupted";
				goto errout;
	        }
			set_inuse_bit_at_offset (victim, nb);
			bin->bk = bck;
			bck->fd = bin;
	        ...
	    }
	}
}