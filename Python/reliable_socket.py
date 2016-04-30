import socket
import time
from os import urandom

class reliable_socket:
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    timeout = 2
    def definetimeout(self, time):
        self.timeout = time
    def bind(self, addr):
        self.sock.bind(addr)
    def sendto(self, msg, addr):
        id = urandom(4)
        self.sock.sendto(bytes('0', 'utf-8') + id + bytes(msg, 'utf-8'), addr)
        time_send = time.clock()
        while time.clock() - time_send < self.timeout:
            self.sock.settimeout(self.timeout - (time.clock() - time_send))
            try:
                packet, address = self.sock.recvfrom(1024)
            except socket.timeout:
                return False
            if len(packet) == 5 and packet[0].decode('utf-8') == '1' and packet[1:5] == id:
                return True
        return False
    def recvfrom(self, bufsize):
        packet, addr = self.sock.recvfrom(bufsize)
        if packet[0].decode('utf-8') == '0':
            self.sock.sendto(bytes('1', 'utf-8') + packet[1:5], addr)
            return (packet[5:len(packet)].decode('utf-8'), addr)
        else:
            return ('', addr)
