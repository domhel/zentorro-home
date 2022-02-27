#include <RCSwitch.h>

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
#define transmitterPin 0 //Pin D3

void setup() {

  Serial.begin(9600);

  // Transmitter-Pin on NodeMCU
  mySwitch.enableTransmit(transmitterPin);
  
  // Optional set protocol (default is 1, will work for most outlets)
  mySwitch.setProtocol(protocol);

  // Optional set pulse length.
  mySwitch.setPulseLength(pulseLength);
  
  // Optional set number of transmission repetitions.
  mySwitch.setRepeatTransmit(3);
  
}

void loop() {
 /* Same switch as above, but tri-state code */ 
  mySwitch.sendTriState(A_An);
  delay(1000);  
  mySwitch.sendTriState(A_Aus);
  delay(1000);
  delay(20000);
}
