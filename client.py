import socket
from time import sleep

host = '192.168.43.112'
port = 1222

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))

while True:
    command = input("Enter Text : ")
    print(command)
    if not command:
        break
    s.sendall(command.encode())
    print("Data Sent")
s.close()   
