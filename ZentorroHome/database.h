#ifndef _DATABASE_H
#define _DATABASE_H

#include <Arduino.h>
#include <Effortless_SPIFFS.h>
#include <ArduinoJson.h>

#define DB_PATH "/database.json"

extern DynamicJsonDocument database;
extern eSPIFFS fileSystem;

void databaseSetup();
void saveDatabase();
void addToDatabase(String name, String state, String code);
void clearDatabase();


#endif
