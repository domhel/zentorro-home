#include "database.h"

DynamicJsonDocument database = DynamicJsonDocument(8192);
eSPIFFS fileSystem = eSPIFFS();

void initDatabase() {
  String db_string;
  if (fileSystem.openFromFile(DB_PATH, db_string)) {
    Serial.println("Successfully read file and parsed data: ");
    deserializeJson(database, db_string);
    Serial.println(db_string);
  } else {
    // DB is new
    Serial.println("Created a new database.");
  }
}

void saveDatabase() {
  String db_string;
  serializeJson(database, db_string);
  Serial.println("Saving database: ");
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
    Serial.println();
    return;
  }

  database[name][state][database[name][state][0]] = code;
  database[name][state][0] = database[name][state][0].as<int32_t>() + 1;
  serializeJson(database, Serial);
  Serial.println();
}

void clearDatabase(){
  
}
