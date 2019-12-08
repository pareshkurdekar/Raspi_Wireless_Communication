import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import sys
Master_subnode = mqtt.Client()

host = '192.168.225.186'
port = 1883

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    Master_subnode.subscribe("esp8266")

    Master_subnode.subscribe("Sensor_values")               #Getting Sensors Values
    Master_subnode.subscribe("Actuation_ACK")

def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    sys.exit()

Master_subnode.connect(host,port,60)

Master_subnode.on_connect = on_connect
Master_subnode.on_message = on_message

while True:
    command = input("Enter Command")
    print(type(command))
        # Master_subnode.publish("esp8266",command)
    if (command == "1"):
        Master_subnode.publish("Sensor_Read_Request",command)
        print("sdsdsd")
    if(command == "2"):
                                            #Decide which actuator to s$
                                            #Write it in the form of a $
        actuation_values = "1:On,2:Off,3:On,4:Off"
        Master_subnode.publish("Actuation",actuation_values,host)

                                                    #Change actuation_values if needed
Master_subnode.loop_forever()
