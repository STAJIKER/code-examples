import socket

HOST = ""
PORT = 5005

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((HOST, PORT))
print("listening on port %s" % PORT)
while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    print("received \"%s\" from %s" % (data, addr))
