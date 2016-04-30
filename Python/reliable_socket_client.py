from reliable_socket import reliable_socket
import sys

HOST = ""
PORT = 5005

r_sock = reliable_socket()
msg = sys.stdin.readline().strip()
b = r_sock.sendto(msg, (HOST, PORT))
if b == True:
    print('Correct')
else:
    print('Error')
