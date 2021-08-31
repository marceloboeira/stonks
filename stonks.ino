#include <LiquidCrystal.h>
#include <Ethernet.h>
#include <ArduinoHttpClient.h>
#include "Debugger.h"
#include "Ticker.h"
#include "Configs.h"

// Debugger Setup
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
HttpClient http = HttpClient(eth, API_SERVER, API_PORT);

// State Machine Setup
enum States {SETUP, IDLE, PAGE_LOAD};
uint8_t state = SETUP;

// Pagination Setup
unsigned long refreshedAt = 0;
const unsigned long refreshInterval = REFRESH_INTERNVAL * 1000; // in Milliseconds
const uint8_t totalPages = TickersCount;
uint8_t currentPage = 0;

void setup() {};

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

void stateSetup() {
  debugger.setup();
  debugger.log("State: Setup");

  lcd.begin(LCD_COLUMNS, LCD_ROWS);
  lcd.setCursor(0, 0);
  lcd.write("stonks!");
  lcd.setCursor(0, 1);
  lcd.write("Network...");
  setupEthernet();

  lcdClearLine(1);
  lcd.setCursor(0, 1);
  lcd.write("Loading...");
  state = IDLE;
}

void statePageLoad() {
  debugger.log("State: Page Load");
  const Ticker ticker = Tickers[currentPage];

  debugger.log("GET " + String(API_RANDOM_PATH));
  http.get(API_RANDOM_PATH);

  int statusCode = http.responseStatusCode();
  String body = http.responseBody();
  debugger.log("  Status Code: " + String(statusCode));
  debugger.log("  Body: " + body);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(ticker.label);

  lcd.setCursor(0, 1);
  if (body.length() >= 3) {
    String change = body.substring(1, body.length() - 1);
    String sign = (change.toInt() > 30) ? "+" : "-";
    String changeM = (sign + "1." + change + "% ");
    String valueM = (String(change.toInt()* 27) + " " + ticker.currency);
    const int spaces = (LCD_COLUMNS - (changeM.length() + valueM.length()));

    lcd.print(changeM);
    lcd.setCursor(changeM.length() + spaces, 1);
    lcd.print(valueM);
  } else {
    lcd.print("Error...");
  }

  currentPage = nextPage();
  state = IDLE;
}

bool timeToRefreshPage() {
  return ((millis() - refreshedAt) > refreshInterval);
}

uint8_t nextPage() {
  if (currentPage >= (totalPages - 1)) {
    return 0;
  }
  return currentPage + 1;
}

void stateIdle() {
  debugger.log("State: Idle");

  if (timeToRefreshPage()) {
    state = PAGE_LOAD;
    refreshedAt = millis();
  }
}

void loop() {
  switch(state) {
    case SETUP:
      stateSetup();
      break;
    case IDLE:
      stateIdle();
      break;
    case PAGE_LOAD:
      statePageLoad();
      break;
  }
}
