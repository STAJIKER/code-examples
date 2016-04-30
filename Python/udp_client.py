import socket
import sys

HOST = ""
PORT = 5005

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
msg = sys.stdin.readline().strip()
sock.sendto(bytes(msg, 'utf-8'), (HOST, PORT))
