/*
 Basic ESP8266 MQTT example
 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Receive.h"
#include "output.h"
#include <ArduinoJson.h>
#include "Transmit.h"
#include "database.h"


// Update these with values suitable for your network.

const char* ssid = "FRITZ!Box 7530 SC";
const char* password = "01178381469567963357";
const char* mqtt_server = "broker.mqttdashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
unsigned long decimal_device;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
char device[64] = "";
char state[64] = "";
String hex_code;
String topic_String;
int test = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");

  StaticJsonDocument<256> doc;
  deserializeJson(doc, (const byte*)payload, length);
  strlcpy(device, doc["device"] | "default", sizeof(device)); 
  strlcpy(state, doc["state"] | "default", sizeof(state)); 

  Serial.println(device);
  Serial.println(state);
  topic_String = String(topic);
  if(topic_String == "433MHzBridge/learn"){
    client.publish("433MHzBridge/status", "ON");
    test = 1;
  }
  else if(topic_String == "433MHzBridge/control"){
    transmit(device, state);
  }
  else if(topic_String == "433MHzBridge/clear"){
    clearDatabase();
    Serial.println("clear");
  }
}

uint8_t sameCodeInRowCount = 0;
String previousCode;
void scan(){ 
  // Dominik: muss nicht in einer Loop abgefragt werden, bis ein Code da ist?
  // ausserdem muss der gleiche Code 3x received werden
  
   if(sameCodeInRowCount < 3) {
    hex_code = receive_code();
    if(hex_code == ""){
      return;
    }
     Serial.println("Got code " + hex_code + " for " + device + " and state " + state);
    if (hex_code == previousCode || sameCodeInRowCount == 0) {
      Serial.println("Got code " + hex_code + " for " + device + " and state " + state);
      ++sameCodeInRowCount;
      Serial.println(sameCodeInRowCount);
    } else {
      previousCode = hex_code;
      sameCodeInRowCount = 0;
    }
    }
    else if(sameCodeInRowCount >= 3){
      test = 0;
      client.publish("433MHzBridge/status", "OFF");
      sameCodeInRowCount = 0;
      previousCode = String("");
      addToDatabase(String(device), String(state), String(hex_code));
      saveDatabase();
    }
  
  /* addToDatabase("Steckdose1", "on", "000000FFFF0F"); */
  /* addToDatabase("Steckdose1", "off", "000000FFFFF0"); */
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      String databaseString;
      serializeJson(database, databaseString);
      client.publish("433MHzBridge/database", databaseString.c_str());
      // ... and resubscribe
      client.subscribe("433MHzBridge/learn");
      client.subscribe("433MHzBridge/control");
      client.subscribe("433MHzBridge/clear");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  //initDatabase();
  transmit_setup();
  receive_setup(); 
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if(test){
    scan(); 
  }
}
