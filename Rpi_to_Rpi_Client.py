import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish

Rpi_Client = mqtt.Client()

host = '192.168.225.186'
port = 1883

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    Rpi_Client.subscribe("Read_Data")

def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    print("Data Read...")


Rpi_Client.connect(host,port,60)

Rpi_Client.on_connect = on_connect
Rpi_Client.on_message = on_message

Rpi_Client.loop_start()

Rpi_Client.subscribe("Read_Data")


while True:
    command = input("Enter Command")
    if (command == "1"):
        str = "Data Request"
        Rpi_Client.publish("Write_Data",str)



Rpi_Client.loop_forever()
