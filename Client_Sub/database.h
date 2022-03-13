#ifndef _DATABASE_H
#define _DATABASE_H

#include <Arduino.h>
#include <Effortless_SPIFFS.h>
#include <ArduinoJson.h>

#define DB_PATH "/database.json"

extern DynamicJsonDocument database(8192);
extern eSPIFFS fileSystem;

void initDatabase();
void saveDatabase();
void addToDatabase(String name, String state, String code);


#endif
