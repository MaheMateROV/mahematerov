import socket
import time

server_ip = "192.168.2.20" # laptop ip
client_ip = "192.168.2.100" # arduino ip
PORT = 12345

server = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
server.bind((server_ip,PORT))
print("server waiting for packets...")

while True:
    try:
        packet, addr = server.recvfrom(1024)
        print(f"packet received from {client_ip} : {packet.decode()}")
    except:
        server.close()
        print("server closed . ")