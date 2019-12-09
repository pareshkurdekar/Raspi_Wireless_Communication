/*****

  All the resources for this project:
  https://randomnerdtutorials.com/

*****/

// Loading the ESP8266WiFi library and the PubSubClient library
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "Xender_AP8616";
const char* password = "12345678";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "192.168.43.17";


//Initializing the Pins for the sensor
int pH_sensor = D0;
int DO_sensor = D1;
int Conductivity_sensor = D2;
int sensor_input = A0;

int sensors[] = {pH_sensor, DO_sensor, Conductivity_sensor};

// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);




// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];// your ESP8266 is subscribed you can actually do something

  }
  Serial.println();


  if (topic == "Sensor_Read_Request")

  {
    Serial.print("Raspberry Pi has pubished: ");
    Serial.println(messageTemp);
    float result;
    String str = "";
    for (int i = 0; i <= 2; ++i)
    {
      char buf[10];

      result = Read_sensor(sensors[i]);
      dtostrf(result, 4, 4, buf);
      str += buf;
      str += ",";

    }


    /* String sensor_string = "Ph,DO,Conductivity Values!";

      unsigned int sensor_result_length = sensor_string.length();
      byte sensor_result[sensor_result_length];
      sensor_string.getBytes(sensor_result, sensor_result_length);
      client.publish("Sensor_values", sensor_result, sensor_result_length);
      Serial.println(sensor_string);
    */
    unsigned int sensor_result_length = str.length();
    byte sensor_result[sensor_result_length];
    str.getBytes(sensor_result, sensor_result_length);
    client.publish("Sensor_values", sensor_result, sensor_result_length);
    Serial.println(str);
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
      if (client.connect("ESP8266Client")) {
      If you want more devices connected to the MQTT broker, you can do it like this:
      if (client.connect("ESPOffice")) {
      Then, for the other ESP:
      if (client.connect("ESPGarage")) {
      That should solve your MQTT multiple connections problem

      THE SECTION IN loop() function should match your device name
    */
    if (client.connect("ESP8266Client"))
    {
      Serial.println("connected");
      // Subscribe or resubscribe to a topic

      client.subscribe("Sensor_Read_Request");


    }
    else
    {
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

  pinMode(pH_sensor, OUTPUT);
  pinMode(DO_sensor, OUTPUT);
  pinMode(Conductivity_sensor, OUTPUT);
  pinMode(sensor_input, INPUT);
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
      client.connect("ESP8266Client");
      If you want more devices connected to the MQTT broker, you can do it like this:
      client.connect("ESPOffice");
      Then, for the other ESP:
      client.connect("ESPGarage");s
      That should solve your MQTT multiple connections problem

      THE SECTION IN recionnect() function should match your device name
    */
    client.connect("ESP8266Client");
}



float Read_sensor(int pin)
{

  digitalWrite(pin, HIGH);
  delay(2000);
  int sensor_value = analogRead(sensor_input);
  Serial.println(sensor_value);
  digitalWrite(pin, LOW);
  return sensor_value;

}

