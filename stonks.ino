#include <Ethernet.h>
#include <ArduinoHttpClient.h>
#include <LiquidCrystal.h>


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 86, 177);
IPAddress myDns(192, 168, 86, 1); 

EthernetClient eth;
char server[] = "www.randomnumberapi.com";  // also change the Host line in httpRequest()
HttpClient http = HttpClient(eth, server, 80);

unsigned long lastConnectionTime = 0;
const unsigned long postingInterval = 10 * 1000;

void setup() {
  lcd.write("stonks!");

  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        ;
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }

    Ethernet.begin(mac, ip, myDns);
    Serial.print("My IP address: ");
    Serial.println(Ethernet.localIP());
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  delay(1000);
}

void loop() {
  if (millis() - lastConnectionTime > postingInterval) {
    Serial.println("making GET request");
    http.get("/api/v1.0/random");

    int statusCode = http.responseStatusCode();
    String response = http.responseBody();

    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);
    lcd.clear();
    lcd.print(response);

    lastConnectionTime = millis();
  }
}
