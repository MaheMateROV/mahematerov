import socket
import time

server_ip = "192.168.2.20" # laptop ip
client_ip = "192.168.2.100" # arduino ip
PORT = 12345

server = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
server.bind((server_ip,PORT))

while True:
	try:
		packet = "hello"
		server.sendto(packet.encode(),(client_ip,PORT))
		print(f"packet sent : {packet}")
		time.sleep(1)
	except:
		server.close()
		print("server closed . ")