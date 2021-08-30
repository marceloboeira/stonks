#include <LiquidCrystal.h>
#include <Ethernet.h>
#include <ArduinoHttpClient.h>
#include "Debugger.h"

// Debugger Setup
Debugger debugger(true, 9600);

// LCD Setup
// ekitszone.com pin setup
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Network Stack
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 86, 177);
IPAddress myDns(192, 168, 86, 1);
EthernetClient eth;

// Server Setup
#define API_SERVER "www.randomnumberapi.com"
#define API_PORT 80
#define API_RANDOM_PATH "/api/v1.0/random"

HttpClient http = HttpClient(eth, API_SERVER, API_PORT);

unsigned long lastConnectionTime = 0;
const unsigned long postingInterval = 10 * 1000;

void setup() {
  debugger.setup();

  lcd.write("stonks!");

  debugger.log("Initialize Ethernet");
  if (Ethernet.begin(mac) == 0) {
    debugger.log("  Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      debugger.log("    FATAL: Ethernet shield was not found.");
      while (true) {
        ;
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      debugger.log("    FATAL: Ethernet cable is not connected.");
    }

    Ethernet.begin(mac, ip, myDns);
    debugger.log("  Fixed IP assgined");
  } else {
    debugger.log("  DHCP assigned IP");
  }
  debugger.log(Ethernet.localIP());
  delay(1000);
}

void loop() {
  if (millis() - lastConnectionTime > postingInterval) {

    debugger.log("GET " + String(API_RANDOM_PATH));
    http.get(API_RANDOM_PATH);

    int statusCode = http.responseStatusCode();
    String body = http.responseBody();
    debugger.log("  Status Code: " + String(statusCode));
    debugger.log("  Body: " + body);

    lcd.clear();
    lcd.print(body);

    lastConnectionTime = millis();
  }
}
