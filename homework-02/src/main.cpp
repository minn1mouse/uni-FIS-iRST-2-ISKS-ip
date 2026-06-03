#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>

#include <secrets.h>
#include <wifi_log.h>

WebServer server(80);

uint8_t LED_BLUE  = 2;
uint8_t LED_RED   = 4;
uint8_t LED_GREEN = 19;

bool blueStatus  = LOW;
bool redStatus   = LOW;
bool greenStatus = LOW;

// EN: read the HTML template from LittleFS, swap the `%MESSAGE%` placeholder
//     for the per-route confirmation text, and send. Same templating
//     pattern as homework-01, just with a single placeholder.
// SL: preberemo HTML predlogo iz LittleFS, zamenjamo oznako `%MESSAGE%` z
//     besedilom za posamezno pot in pošljemo. Enak vzorec predloge kot v
//     homework-01, le z eno samo oznako.
void sendPage(const char* message) {
  File f = LittleFS.open("/index.html", "r");
  if (!f) {
    server.send(500, "text/plain; charset=UTF-8",
                "Manjka /index.html — zaženi `pio run --target uploadfs`");
    return;
  }
  String page = f.readString();
  f.close();
  page.replace("%MESSAGE%", message);
  server.send(200, "text/html; charset=UTF-8", page);
}

void handle_root() {
  sendPage("Pritisni gumb (/0 do /5) za upravljanje LED diod."
           "<br>/0,/1 = modra | /2,/3 = rdeca | /4,/5 = zelena");
}

void handle_1() { blueStatus  = HIGH; sendPage("Prejet ukaz za vklop modre LED diode."); }
void handle_0() { blueStatus  = LOW;  sendPage("Prejet ukaz za izklop modre LED diode."); }
void handle_3() { redStatus   = HIGH; sendPage("Prejet ukaz za vklop rdece LED diode."); }
void handle_2() { redStatus   = LOW;  sendPage("Prejet ukaz za izklop rdece LED diode."); }
void handle_5() { greenStatus = HIGH; sendPage("Prejet ukaz za vklop zelene LED diode."); }
void handle_4() { greenStatus = LOW;  sendPage("Prejet ukaz za izklop zelene LED diode."); }

void setup() {
  Serial.begin(115200);

  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed — zaženi `pio run --target uploadfs`");
  } else {
    Serial.println("LittleFS priklopljen.");
  }

  printWifiTarget();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Povezovanje z WiFi omrežjem...");
  }

  Serial.println("Povezava z WiFi omrežjem je vzpostavljena.");
  Serial.print("Moj IP naslov je: ");
  Serial.println(WiFi.localIP());

  server.on("/",  handle_root);
  server.on("/0", handle_0);  // modra OFF
  server.on("/1", handle_1);  // modra ON
  server.on("/2", handle_2);  // rdeca OFF
  server.on("/3", handle_3);  // rdeca ON
  server.on("/4", handle_4);  // zelena OFF
  server.on("/5", handle_5);  // zelena ON

  server.begin();

  pinMode(LED_BLUE,  OUTPUT);
  pinMode(LED_RED,   OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  digitalWrite(LED_BLUE, HIGH); delay(250);
  digitalWrite(LED_BLUE, LOW);  delay(250);
  digitalWrite(LED_BLUE, HIGH); delay(250);
  digitalWrite(LED_BLUE, LOW);  delay(250);
}

void loop() {
  server.handleClient();

  digitalWrite(LED_BLUE,  blueStatus  ? HIGH : LOW);
  digitalWrite(LED_RED,   redStatus   ? HIGH : LOW);
  digitalWrite(LED_GREEN, greenStatus ? HIGH : LOW);
}
