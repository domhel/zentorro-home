#include <RCSwitch.h>
#include <Effortless_SPIFFS.h>
#include <ArduinoJson.h>
#include "Transmit.h"


RCSwitch mySwitch = RCSwitch();

#define A_An "000000FFFF0F"
#define B_An "00000F0FFF0F"
#define C_An "00000FF0FF0F"
#define D_An "00000FFF0F0F"

#define A_Aus "000000FFFFF0"
#define B_Aus "00000F0FFFF0"
#define C_Aus "00000FF0FFF0"
#define D_Aus "00000FFF0FF0"

#define pulseLength 324
#define protocol 1
#define repeat 3
#define transmitterPin D3

#define DB_PATH "/database.json"

DynamicJsonDocument database(8192);
eSPIFFS fileSystem;
bool isRecordingCodes = false;

void transmit_setup() {

  // Transmitter-Pin on NodeMCU
  mySwitch.enableTransmit(transmitterPin);
  
  // Optional set protocol (default is 1, will work for most outlets)
  mySwitch.setProtocol(protocol);

  // Optional set pulse length.
  mySwitch.setPulseLength(pulseLength);
  
  // Optional set number of transmission repetitions.
  mySwitch.setRepeatTransmit(3);

  String db_string;
  if (fileSystem.openFromFile(DB_PATH, db_string)) {
    Serial.print("Successfully read file and parsed data: ");
    deserializeJson(database, db_string);
    Serial.println(db_string);
    database["Flur"]["off"][0] = A_Aus;
    saveDatabase();
  } else {
    // DB is new
  }

  addToDatabase("Zweiter Eintrag", "off", B_Aus);
  
}

void transmit_loop() {
 /* Same switch as above, but tri-state code */ 
  mySwitch.sendTriState(A_An);
  Serial.println("ON");
  delay(1000);  
  mySwitch.sendTriState(A_Aus);
  Serial.println("OFF");
  delay(1000);
  delay(20000);
}


void saveDatabase() {
  String db_string;
  serializeJson(database, db_string);
  Serial.print("Saving database: ");
  Serial.println(db_string);
  if (fileSystem.saveToFile(DB_PATH, db_string)) {
      Serial.println("Saving complete.");
  } else {
    Serial.println("Error saving database.");
  }
}

void addToDatabase(String name, String state, String code) {
  if (state != "off" && state != "on") return;

  if (!database.containsKey(name)) {
    // Name is new
    database[name]["on"][0] = 1; // next code will be placed at this index
    database[name]["off"][0] = 1;
    database[name][state][1] = code;
    database[name][state][0] = 2;
    serializeJson(database, Serial); // TODO: remove in production
    return;
  }

  database[name][state][database[name][state][0]] = code;
  database[name][state][0] = database[name][state][0].as<int32_t>() + 1;
  serializeJson(database, Serial);
}
