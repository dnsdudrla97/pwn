struct _IO_FILE
{
  int _flags;		/* High-order word -> _IO_MAGIC, 나머지는 플래그 */
  /* 다음 포인터는 C++ streambuf 프로토콜에 해당한다. */
  char *_IO_read_ptr;	/* 파일 읽기 버퍼에 대한 포인터*/
  char *_IO_read_end;	/* 파일 읽기 버퍼 주소의 끝을 가리키는 포인터*/
  char *_IO_read_base;	/* Start of putback+get area., 파일 읽기 버퍼 주소의 시작을 가리키는 포인터 */
  char *_IO_write_base;	/* 파일 쓰기 버퍼의 주소의 시작을 가리키는 포이넡 */
  char *_IO_write_ptr;	/* 쓰기 버퍼에 대한 포인터 */
  char *_IO_write_end;	/* 파일 쓰기 버퍼 주소의 끝을 가리키는 포인터 */
  char *_IO_buf_base;	/* Start of reserve area. 준비 구역의 시작 */
  char *_IO_buf_end;	/* 준비 구역의 끝 */
  /* 다음 필드는 백업 및 실행 취소를 지원하는데 사용된다.*/
  char *_IO_save_base; /* Pointer to start of non-current get area. */
  char *_IO_backup_base;  /* Pointer to first valid character of backup area */
  char *_IO_save_end; /* Pointer to end of non-current get area. */
  struct _IO_marker *_markers;
  struct _IO_FILE *_chain; 
  int _fileno;
  int _flags2;
  __off_t _old_offset; /* This used to be _offset but it's too small.  */
  /* 1+column number of pbase(); 0 is unknown. */
  unsigned short _cur_column;
  signed char _vtable_offset;
  char _shortbuf[1];
  _IO_lock_t *_lock;
#ifdef _IO_USE_OLD_IO_FILE
};