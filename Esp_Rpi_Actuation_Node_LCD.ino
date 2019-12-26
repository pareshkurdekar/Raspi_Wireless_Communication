/*****

  All the resources for this project:
  https://randomnerdtutorials.com/

*****/

// Loading the ESP8266WiFi library and the PubSubClient library
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "21 rs ghants";
const char* password = "";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "192.168.43.17";

// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);

// Connect an LED to each GPIO of your ESP8266
int relay_1 = D6;
int relay_2 = D5;
int relay_3 = D3;
int relay_4 = D0;
int Logic_3v3 = D4;
int relay_out[4];  //value from Rpi

byte actuation_result[4];

int Num_relay = 4;
int relay_val;
int relay_pos[] = {relay_1, relay_2, relay_3, relay_4};
// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");

  lcd.setCursor(0, 0);
  lcd.print("Connecting to ");

  Serial.println(ssid);
  lcd.setCursor(1, 1);
  lcd.print(ssid);
  delay(1000);
  lcd.clear();
  int l = 0;

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    lcd.setCursor(l, 0);
    lcd.print(".");
    ++l;
  }

  delay(1000);

  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
  lcd.setCursor(0, 0);
  lcd.print("ESP address");

  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(1000);
}

// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that
void callback(String topic, byte* message, unsigned int length) {
  lcd.clear();
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);

  lcd.setCursor(0, 0);
  lcd.print("Topic: ");
      
  lcd.setCursor(6, 0);
  lcd.print(topic);
  
  Serial.print(". Message: ");
  Serial.print("length"); Serial.println(length);
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];// your ESP8266 is subscribed you can actually do something
    relay_val = message[i] - '0' ;
    
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic home/office/esp1/gpio2, you check if the message is either 1 or 0. Turns the ESP GPIO according to the message
  if (topic == "node_1/actuator_subnode/actuator1")

  {

    Serial.print("Raspberry Pi has published: ");
    Serial.println(messageTemp);
    
    relay_output(0, !relay_val);
    relay_out[0] = relay_val;
 /*   for (int i = 0; i < length; ++i)
    {
      //   Serial.println(relay_out[i]);
      relay_output(i, relay_out[i]);
      delay(100);
    }
*/
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

  /*  for (int i = 0; i <= length; ++i)
    {
      actuation_result[i] = digitalRead(relay_pos[i]);

    }
    client.publish("Actuation_ACK", actuation_result, length);

   // lcd.clear();
   // lcd.setCursor(0, 0);
   // lcd.print("ACK Published");
*/
relay_read();
  
  }
 else if (topic == "node_1/actuator_subnode/actuator2")

  {

    Serial.print("Raspberry Pi has published: ");
    Serial.println(messageTemp);
    
    relay_output(1, !relay_val);
    relay_out[1] = relay_val;
    delay(2000);
    relay_read();
  
  }
  else if (topic == "node_1/actuator_subnode/actuator3")

  {

    Serial.print("Raspberry Pi has published: ");
    Serial.println(messageTemp);

        relay_output(2, !relay_val);
    relay_out[2] = relay_val;
  delay(2000);
    relay_read();
  
  }
 else if (topic == "node_1/actuator_subnode/actuator4")

  {

    Serial.print("Raspberry Pi has published: ");
    Serial.println(messageTemp);
  delay(2000);    
  relay_output(3, !relay_val);
    relay_out[3] = relay_val;
    relay_read();
  
  }



  Serial.println();
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    lcd.clear();
    Serial.print("Attempting MQTT connection...");
    lcd.setCursor(0, 0);
    lcd.print("Attempting MQTT");
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

      lcd.setCursor(0, 1);
      lcd.print("Connected!");
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      //   client.subscribe("esp8266");
      //   client.subscribe("Sensor_Read_Request");
      client.subscribe("node_1/actuator_subnode/actuator1");
      client.subscribe("node_1/actuator_subnode/actuator2");
      client.subscribe("node_1/actuator_subnode/actuator3");
      client.subscribe("node_1/actuator_subnode/actuator4");
      //client.subscribe("node_1/actuator_subnode/actuator1");

    } else {
      Serial.print("failed, rc=");
      lcd.setCursor(0, 1);
      lcd.print("Failed!");
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

  pinMode(Logic_3v3,OUTPUT);
  
  digitalWrite(Logic_3v3,HIGH);
  digitalWrite(relay_1,HIGH);
  digitalWrite(relay_2,HIGH);
  digitalWrite(relay_3,HIGH);
  digitalWrite(relay_4,HIGH);
  lcd.init();                      // initialize the lcd
  lcd.backlight();

  Serial.begin(115200);

  setup_wifi();
  delay(5000);
 // digitalWrite(relay_pos[0],LOW);
 // digitalWrite(relay_pos[1],LOW);
 // digitalWrite(relay_pos[2],LOW);
 // digitalWrite(relay_pos[3],LOW);
   

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
  
    client.connect("ESP8266Act");
}



void relay_output(int pos, int val)
{
  Serial.print("Pos :");
  Serial.println(pos);
    Serial.print("Value :");
  Serial.println(val);
  digitalWrite(relay_pos[pos], val);
  Serial.print(digitalRead(relay_pos[pos])); Serial.print("  "); Serial.println(val);

}

void relay_read()
{


    for (int i = 0; i <=(Num_relay-1);++i)
    {
      actuation_result[i] = !(digitalRead(relay_pos[i]));

    }
    
    client.publish("node_1/actuator_subnode/actuation_ACK", actuation_result, Num_relay);
    Serial.println("ACtuaskdjn");
  
}
