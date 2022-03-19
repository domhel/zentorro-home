#include <RCSwitch.h>
#include "Transmit.h"
#include "database.h"

RCSwitch mySwitch2 = RCSwitch();

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


bool isRecordingCodes = false;

void transmitSetup() {

  // Transmitter-Pin on NodeMCU
  mySwitch2.enableTransmit(transmitterPin);
  
  // Optional set protocol (default is 1, will work for most outlets)
  mySwitch2.setProtocol(protocol);

  // Optional set pulse length.
  mySwitch2.setPulseLength(pulseLength);
  
  // Optional set number of transmission repetitions.
  mySwitch2.setRepeatTransmit(3);

}


void transmit(String name, String state){
  if (!database.containsKey(name)) {
    Serial.print("Device name not in database: ");
    Serial.println(name);
    return;
  }
  if (!database[name].containsKey(state)) {
    Serial.print("State not in database: ");
    Serial.println(state);
    return;
  }
  if (database[name][state][0] == 1) {

    Serial.print("State not in database: ");

    Serial.println(state);

    return;

  }
   mySwitch2.sendTriState(database[name][state][1]);
}
