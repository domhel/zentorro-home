#include "Receive.h"
#include "output.h"
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void receive_setup() { mySwitch.enableReceive(D2); }

unsigned long receive_code() {
  if (mySwitch.available()) {
    output(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(),
           mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(),
           mySwitch.getReceivedProtocol());
    mySwitch.resetAvailable();
    return mySwitch.getReceivedValue();
  }
}
