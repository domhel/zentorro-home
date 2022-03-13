#include <RCSwitch.h>
#include "Receive.h"
#include "output.h"

RCSwitch mySwitch = RCSwitch();

void receive_setup() {
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => D3
}

unsigned long receive_code() {
  if (mySwitch.available()) {
    output(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(),mySwitch.getReceivedProtocol());
    mySwitch.resetAvailable();
    return mySwitch.getReceivedValue();
  }
}