#include "database.h"

DynamicJsonDocument database = DynamicJsonDocument(8192);
eSPIFFS fileSystem = eSPIFFS();

void databaseSetup() {
  String dbString;
  if (fileSystem.openFromFile(DB_PATH, dbString)) {
    Serial.println("Successfully read file and parsed data: ");
    deserializeJson(database, dbString);
    Serial.println(dbString);
  } else {
    // DB is new
    Serial.println("Created a new database.");
  }
}

void saveDatabase() {
  String dbString;
  serializeJson(database, dbString);
  Serial.println("Saving database: ");
  Serial.println(dbString);
  if (fileSystem.saveToFile(DB_PATH, dbString)) {
      Serial.println("Saving complete.");
  } else {
    Serial.println("Error saving database.");
  }
}



void addToDatabase(String name, String state, String code) {
  bool containsName = database.containsKey(name);
  bool containsState = false;
  if (containsName) {
    containsState = database[name].containsKey(state);
  }
  if (!containsName || !containsState) {
    // Name or state is new
    database[name][state][0] = 2; // next code will be placed at this index
    database[name][state][1] = code;
    serializeJson(database, Serial); // TODO: remove in production
    Serial.println();
    return;
  }
  // Name and state are already in the database
  // Check if the code is already in the database
  int index = database[name][state][0];
  String previousCode = database[name][state][index];
  if (previousCode == code) {
    // Code is already in the database
    return;
  }
  // Code is not in the database
  // Add code to database
  database[name][state][database[name][state][0]] = code;
  database[name][state][0] = database[name][state][0].as<int32_t>() + 1;
  serializeJson(database, Serial);
  Serial.println();
}

void clearDatabase(){
  database.clear();
  saveDatabase();
}
