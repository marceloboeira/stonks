#include <Ethernet.h>
#include "Debugger.h"

Debugger::Debugger(bool enabled, int port) {
  this->serialEnabled = enabled;
  this->serialPort = port;
}

void Debugger::setup() {
  if (this->serialEnabled) {
    Serial.begin(this->serialPort);
    while (!Serial) {
      ;
    }
    log("Initialize Serial Port");
  }
};

void Debugger::log(String message) {
  if (this->serialEnabled) {
    Serial.println(message);
  }
};

void Debugger::log(IPAddress ipAddress) {
  log(String(ipAddress[0]) + String(".") +
       String(ipAddress[1]) + String(".") +
       String(ipAddress[2]) + String(".") +
       String(ipAddress[3]));
};
