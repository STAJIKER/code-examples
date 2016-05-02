from reliable_socket import reliable_socket

HOST = ""
PORT = 5005

r_sock = reliable_socket()
r_sock.bind((HOST, PORT))
print("listening on port %s" % PORT)
while True:
    data, addr = r_sock.recvfrom(1024)
    if data == '':
        print('Incorrect packet')
    else:
        print("received \"%s\" from %s" % (data, addr))
