#include "Receive.h"
#include "output.h"
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void receiveSetup() { mySwitch.enableReceive(D2); }
String receiveCode() {
  if(mySwitch.available()) {
    const char* b = dec2binWzerofill(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength());
    const char* hex_code = bin2tristate( b);
    mySwitch.resetAvailable(); 
    return String(hex_code);
  }
  return String("");
}
