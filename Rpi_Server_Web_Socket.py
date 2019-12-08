import socket

host = ''
port = 1222


def setupServer():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print("Socket created.")
    try:
        s.bind((host, port))
    except socket.error as msg:
        print(msg)
    print("Socket bind complete.")
    return s

def setupConnection():
    s.listen(1)
    conn, address = s.accept()
    print("Connected to: " + address[0] + ":" + str(address[1]))
    return conn

def dataTransfer(conn):
    # A big loop that sends/receives data until told not to.
    while True:
        # Receive the data
        data = conn.recv(2048) # receive the data
        if not data:
            break
        conn.sendall(data)
        data = data.decode('utf-8')

        print("Server received ", data)
        print("Data has been sent!")
    s.close()


s = setupServer()

while True:
    try:
        conn = setupConnection()
        dataTransfer(conn)
    except:
        print("Error!")
        break
