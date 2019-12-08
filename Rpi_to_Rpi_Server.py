import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish

Rpi_Server = mqtt.Client()

host = '192.168.225.186'
port = 1883

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    Rpi_Server.subscribe("Write_Data")

def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    print("Data Read...")


Rpi_Server.connect(host,port,60)

Rpi_Server.on_connect = on_connect
Rpi_Server.on_message = on_message

Rpi_Server.loop_start()

Rpi_Server.subscribe("Write_Data")


while True:
    command = input("Enter Command")
    if (command == "1"):
        str = "Data is written"
        Rpi_Server.publish("Read_Data",str)
        


Rpi_Server.loop_forever()
