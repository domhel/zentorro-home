#ifndef OUTPUT_H
#define OUTPUT_H
#include <Arduino.h>

void transmit_setup();
void transmit(String name, String state);
void saveDatabase();
void addToDatabase(String name, String state, String code);


#endif
