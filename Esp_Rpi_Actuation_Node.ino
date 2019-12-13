/*****

  All the resources for this project:
  https://randomnerdtutorials.com/

*****/

// Loading the ESP8266WiFi library and the PubSubClient library
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "JioFi3_Basil";
const char* password = "12345679";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "192.168.225.239";

// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);

// Connect an LED to each GPIO of your ESP8266
int relay_1 = D5;
int relay_2 = D4;
int relay_3 = D3;
int relay_4 = D0;
int relay_out[4];  //value from Rpi

byte actuation_result[4];

int relay_pos[] = {relay_1, relay_2, relay_3, relay_4};
// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");



  Serial.println(ssid);
 

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
 
  }



  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
  
  
  delay(100);
}

// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that
void callback(String topic, byte* message, unsigned int length) {

  Serial.print("Message arrived on topic: ");
  Serial.print(topic);

 
  
  Serial.print(". Message: ");
  Serial.print("length"); Serial.println(length);
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];// your ESP8266 is subscribed you can actually do something
    relay_out[i] = message[i] - '0' ;
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic home/office/esp1/gpio2, you check if the message is either 1 or 0. Turns the ESP GPIO according to the message
  if (topic == "Actuation")

  {

    Serial.print("Raspberry Pi has published: ");
    Serial.println(messageTemp);

    for (int i = 0; i < length; ++i)
    {
      //   Serial.println(relay_out[i]);
      relay_output(i, relay_out[i]);
      delay(100);
    }

    /*

      READ SENSOR VALUES AND PUBLISH IT TO THE RASPBERRY PI

    */
    delay(3000);

    /*String actuation_string = "Relay Signals!";

      unsigned int actuation_result_length = actuation_string.length();
      byte actuation_result[actuation_result_length];
      actuation_string.getBytes(actuation_result, actuation_result_length);
      client.publish("Actuation_ACK", actuation_result, actuation_result_length);
    */

    //Serial.println(actuation_string);

    for (int i = 0; i <= length; ++i)
    {
      actuation_result[i] = digitalRead(relay_pos[i]);

    }
    client.publish("Actuation_ACK", actuation_result, length);

  
  }



  Serial.println();
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    
    Serial.print("Attempting MQTT connection...");
    

    // Attempt to connect
    /*
      YOU  NEED TO CHANGE THIS NEXT LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
      To change the ESP device ID, you will have to give a unique name to the ESP8266.
      Here's how it looks like now:
      if (client.connect("ESP8266Act")) {
      If you want more devices connected to the MQTT broker, you can do it like this:
      if (client.connect("ESPOffice")) {
      Then, for the other ESP:
      if (client.connect("ESPGarage")) {
      That should solve your MQTT multiple connections problem

      THE SECTION IN loop() function should match your device name
    */
    if (client.connect("ESP8266Act")) {
      Serial.println("connected");

      
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      //   client.subscribe("esp8266");
      //   client.subscribe("Sensor_Read_Request");
      client.subscribe("Actuation");

    } else {
      Serial.print("failed, rc=");
     
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
void setup() {

  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);
  pinMode(relay_3, OUTPUT);
  pinMode(relay_4, OUTPUT);


  Serial.begin(115200);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// For this project, you don't need to change anything in the loop function.
// Basically it ensures that you ESP is connected to your broker
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if (!client.loop())
    /*
      YOU  NEED TO CHANGE THIS NEXT LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
      To change the ESP device ID, you will have to give a unique name to the ESP8266.
      Here's how it looks like now:
      client.connect("ESP8266Act");
      If you want more devices connected to the MQTT broker, you can do it like this:
      client.connect("ESPOffice");
      Then, for the other ESP:
      client.connect("ESPGarage");s
      That should solve your MQTT multiple connections problem

      THE SECTION IN recionnect() function should match your device name
    */
    client.connect("ESP8266Act");
}



void relay_output(int pos, int val)
{
  digitalWrite(relay_pos[pos], val);
  Serial.print(digitalRead(relay_pos[pos])); Serial.print("  "); Serial.println(val);

}
