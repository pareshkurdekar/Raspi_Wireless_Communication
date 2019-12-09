import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import time

Master_subnode = mqtt.Client()

host =  "192.168.43.17"
port = 1883
ctr = 0

pH = 0.0
DO = 0.0
Conductivity = 0.0
sensor_value_list = []

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))


    Master_subnode.subscribe("Sensor_values")               #Getting Sensors Values

    Master_subnode.subscribe("Actuation_ACK")


def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    s = msg.payload.decode("utf-8")
    print(s,type(s))
    if msg.topic == "Sensor_values":
        diff_sensor(s)

#Master_subnode.connect(host,port,60)

def diff_sensor(s):
    temp_list = list(s)
    global sensor_value_list
    for i in range(0,len(s)):
        if s[i] is ",":
            sensor_value_list += s[i - 1]
    print(sensor_value_list)        


Master_subnode.on_connect = on_connect
Master_subnode.on_message = on_message
Master_subnode.connect(host,port,60)
Master_subnode.loop_start()
#Master_subnode.subscribe("Sensor_Values")

while True:
    time.sleep(0.5)
    command = input("Enter Command :")


    if (command == "1"):
        Master_subnode.publish("Sensor_Read_Request",command)
        time.sleep(6)
    if(command == "2"):
                                            #Decide which actuator to s$
                                            #Write it in the form of a $
        actuation_values = "1:On,2:Off,3:On,4:Off"
        Master_subnode.publish("Actuation",actuation_values)
