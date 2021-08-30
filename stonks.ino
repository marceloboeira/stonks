#include <LiquidCrystal.h>
#include <Ethernet.h>
#include <ArduinoHttpClient.h>
#include "Debugger.h"

// Debugger Setup
#define DEBUGGER_ENABLED true
#define DEBUGGER_PORT 9600
Debugger debugger(DEBUGGER_ENABLED, DEBUGGER_PORT);

// LCD Setup
// ekitszone.com pin setup
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Ethernet Setup
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 86, 177);
IPAddress myDns(192, 168, 86, 1);
EthernetClient eth;

// Server Setup
#define API_SERVER "www.randomnumberapi.com"
#define API_PORT 80
#define API_RANDOM_PATH "/api/v1.0/random?min=0&max=99"
HttpClient http = HttpClient(eth, API_SERVER, API_PORT);

unsigned long lastConnectionTime = 0;
const unsigned long postingInterval = 5 * 1000;

void setup() {
  lcd.begin(16, 2);
  debugger.setup();

  lcd.setCursor(0, 0);
  lcd.write("stonks!");
  lcd.setCursor(0, 1);
  lcd.write("Network...");
  setupEthernet();
}

void setupEthernet() {
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


void lcdClearLine(int line) {
  lcd.setCursor(0, line);
  for (int n = 0; n < 16; n++) {
    lcd.print(" ");
  }
}

void loop() {
  if (millis() - lastConnectionTime > postingInterval) {

    debugger.log("GET " + String(API_RANDOM_PATH));
    http.get(API_RANDOM_PATH);

    int statusCode = http.responseStatusCode();
    String body = http.responseBody();
    debugger.log("  Status Code: " + String(statusCode));
    debugger.log("  Body: " + body);

    lcdClearLine(1);
    lcd.setCursor(0, 1);
    if (body.length() >= 3) {
      lcd.print("AAPL: +1." + body.substring(1, body.length() - 1) + "%");
    } else {
      lcd.print("Error...");
    }

    lastConnectionTime = millis();
  }
}
