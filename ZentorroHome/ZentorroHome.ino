#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "Receive.h"
#include "output.h"
#include "Transmit.h"
#include "database.h"

// Update these with values suitable for your network.
const char* ssid = "your ssid";             
const char* password = "your password";     
const char* mqttServerUrl = "broker.mqttdashboard.com";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void wifiSetup() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

char device[64] = "";
char state[64] = "";
bool codeScanningEnabled = 0;

void onMqttMessageReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");

  StaticJsonDocument<256> doc;
  deserializeJson(doc, (const byte*)payload, length);
  strlcpy(device, doc["device"] | "default", sizeof(device)); 
  strlcpy(state, doc["state"] | "default", sizeof(state)); 

  Serial.println(device);
  Serial.println(state);
  String topicString = String(topic);
  if(topicString == "433MHzBridge/learn"){
    mqttClient.publish("433MHzBridge/status", "ON");
    codeScanningEnabled = 1;
  }
  else if(topicString == "433MHzBridge/control"){
    transmit(device, state);
  }
  else if(topicString == "433MHzBridge/clear"){
    clearDatabase();
    Serial.println("clear");
  }
}

void scanRCCode(){ 
  static uint8_t sameCodeInRowCount = 0;
  static String previousCode = "";
  static String hexCode = "";
  if(sameCodeInRowCount < 3) {
    hexCode = receiveCode();
    if(hexCode == ""){
      return;
    }
    Serial.println("Got code " + hexCode + " for " + device + " and state " + state);
    if (hexCode == previousCode || sameCodeInRowCount == 0) {
      ++sameCodeInRowCount;
      Serial.println(sameCodeInRowCount);
    } else {
      previousCode = hexCode;
      sameCodeInRowCount = 0;
    }
  }
  else if(sameCodeInRowCount >= 3){
    codeScanningEnabled = 0;
    mqttClient.publish("433MHzBridge/status", "OFF");
    sameCodeInRowCount = 0;
    previousCode = String("");
    addToDatabase(String(device), String(state), hexCode);
    saveDatabase();
  }
}


void reconnectMqtt() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      String databaseString;
      serializeJson(database, databaseString);
      mqttClient.publish("433MHzBridge/database", databaseString.c_str());
      // ... and resubscribe
      mqttClient.subscribe("433MHzBridge/learn");
      mqttClient.subscribe("433MHzBridge/control");
      mqttClient.subscribe("433MHzBridge/clear");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  databaseSetup();
  randomSeed(micros()); // reading the database might take a random amount of time
  wifiSetup();
  transmitSetup();
  receiveSetup(); 
  mqttClient.setServer(mqttServerUrl, 1883);
  mqttClient.setCallback(onMqttMessageReceived);
}

void loop() {
  if (!mqttClient.connected()) {
    reconnectMqtt();
  }
  mqttClient.loop();
  if(codeScanningEnabled){
    scanRCCode(); 
  }
}
