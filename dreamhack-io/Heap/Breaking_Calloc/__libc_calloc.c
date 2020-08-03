mem = _int_malloc (av, sz);
...
csz = chunksize (p);
d = (INTERNAL_SIZE_T *) mem;
  clearsize = csz - SIZE_SZ;
  nclears = clearsize / sizeof (INTERNAL_SIZE_T);
  assert (nclears >= 3);
  if (nclears > 9)
    return memset (d, 0, clearsize);