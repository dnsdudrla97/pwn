#!/usr/bin/env python

import socket

target = "192.168.111.143"
port = 8080

prefix = "HELP "
buffer = "A" * 100

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((target, port))
print (sock.recv(1024))
sock.send(prefix + buffer)
print (sock.recv(1024))
sock.close()
