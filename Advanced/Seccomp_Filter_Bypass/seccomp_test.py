#seccomp_test.py
from pwn import *

p = process("./seccomp")
payload = asm("mov eax, 2")
payload += asm("syscall")
p.sendline(payload)
p.interactive()