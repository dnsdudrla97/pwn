static void *
_int_malloc (mstate av, size_t bytes)
{
  ...
  for (;; )
    {
      int iters = 0;
      while ((victim = unsorted_chunks (av)->bk) != unsorted_chunks (av))
        {
          bck = victim->bk;
          ...
          size = chunksize (victim);
          ...
          unsorted_chunks (av)->bk = bck;
          bck->fd = unsorted_chunks (av);
          ...
          if (in_smallbin_range (size))
            {
              ...
            }
          else
            {
              victim_index = largebin_index (size);
              bck = bin_at (av, victim_index);
              fwd = bck->fd;
              /* maintain large bins in sorted order */
              if (fwd != bck)
                {
                  /* Or with inuse bit to speed comparisons */
                  size |= PREV_INUSE;
                  /* if smaller than smallest, bypass loop below */
                  assert (chunk_main_arena (bck->bk));
                  if ((unsigned long) (size)  < (unsigned long) chunksize_nomask (bck->bk))
                    {
                      ...
                    }
                  else
                    {
                      assert (chunk_main_arena (fwd));
                      while ((unsigned long) size < chunksize_nomask (fwd))
                        {
                          fwd = fwd->fd_nextsize;
                          assert (chunk_main_arena (fwd));
                        }
                      if ((unsigned long) size == (unsigned long) chunksize_nomask (fwd))
                        ...
                      else
                        {
                          victim->fd_nextsize = fwd;
                          victim->bk_nextsize = fwd->bk_nextsize;
                          fwd->bk_nextsize = victim;
                          victim->bk_nextsize->fd_nextsize = victim;
                        }
                      bck = fwd->bk;
                    }
                }
              else
                ...
            }
          mark_bin (av, victim_index);
          victim->bk = bck;
          victim->fd = fwd;
          fwd->bk = victim;
          bck->fd = victim;
          #define MAX_ITERS       10000
          if (++iters >= MAX_ITERS)
            break;
        }
    ...
}