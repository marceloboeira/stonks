#ifndef DEBUGGER_H
#define DEBUGGER_H
#include <Ethernet.h>

class Debugger {
  public:
    Debugger(bool enabled, int port);

    void setup();
    void log(String message);
    void log(IPAddress ipAddress);

    private:
      bool serialEnabled = false;
      int serialPort = 9600;
};

#endif
