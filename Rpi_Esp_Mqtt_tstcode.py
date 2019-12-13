

import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import time


Master_subnode = mqtt.Client()

host =  "192.168.225.239"
port = 1883
ctr = 0

pH = 0.0
DO = 0.0
Conductivity = 0.0
sensor_value_list = []

actuation_values = ""
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))


    Master_subnode.subscribe("Sensor_values")               #Getting Sensors Values

    Master_subnode.subscribe("Actuation_ACK")


def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    #print(type(msg.payload))
    s = msg.payload.decode("utf-8")
    #print(s,type(s))
 
    if msg.topic == "Sensor_values":
        diff_sensor(s)
    if msg.topic == "Actuation_ACK":
        diff_actuators(s)

def diff_actuators(s):

    global actuation_values
    d = "".join(str(ord(c)) for c in s) 
    #print(d)
    if actuation_values == d:
        print("Job Done")
    
    print("Acknowledged!")

def diff_sensor(s):

    

    temp_list = s.split(",")
    temp_list[2] = temp_list[2].replace('\x00','')  
    #print(temp_list)
    global sensor_value_list
    for i in range(0,3):

        sensor_value_list.append(float(temp_list[i]))
    print(sensor_value_list)        

#Master_subnode.connect(host,port,60)


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
        print("Sd")
        time.sleep(6) 
    if(command == "2"):
                                            #Decide which actuator to s$
        #global actuation_values                                    #Write it in the form of a $
        actuation_values = "1111"
        Master_subnode.publish("Actuation",actuation_values)
        time.sleep(5)  
                                                    #Change actuation_values if needed


#Master_subnode.loop_forever()
