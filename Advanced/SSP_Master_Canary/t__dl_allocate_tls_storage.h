oid *
    internal_function
    _dl_allocate_tls_storage(void)
{
    void *result;
    size_t size = GL(dl_tls_static_size);
#if TLS_DTV_AT_TP
    /* Memory layout is:
     [ TLS_PRE_TCB_SIZE ] [ TLS_TCB_SIZE ] [ TLS blocks ]
			  ^ This should be returned.  */
    size += (TLS_PRE_TCB_SIZE + GL(dl_tls_static_align) - 1) & ~(GL(dl_tls_static_align) - 1);
#endif
    /* Allocate a correctly aligned chunk of memory.  */
    result = __libc_memalign(GL(dl_tls_static_align), size);