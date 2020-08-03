#define fread(p, m, n, s) _IO_fread (p, m, n, s)
size_t
_IO_fread (void *buf, size_t size, size_t count, FILE *fp) {
    ...
    bytes_read = _IO_sgetn (fp, (char *) buf, bytes_requested);
    ...
}
size_t
_IO_sgetn (FILE *fp, void *data, size_t n)
{
  /* FIXME handle putback buffer here! */
  return _IO_XSGETN (fp, data, n);
}
#define _IO_XSGETN(FP, DATA, N) JUMP2 (__xsgetn, FP, DATA, N)
#define JUMP2(FUNC, THIS, X1, X2) (_IO_JUMPS_FUNC(THIS)->FUNC) (THIS, X1, X2)
# define _IO_JUMPS_FUNC(THIS) (IO_validate_vtable (_IO_JUMPS_FILE_plus (THIS)))