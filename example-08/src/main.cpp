#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>

WebServer server(80);

const char* AP_SSID     = "esp32_1";
const char* AP_PASSWORD = "12345678";

uint8_t LED1pin = 2;
bool LED1status = LOW;

void sendFile(const char* path) {
  File f = LittleFS.open(path, "r");
  if (!f) {
    server.send(404, "text/plain; charset=UTF-8", "Datoteka ne obstaja");
    return;
  }
  server.streamFile(f, "text/html; charset=UTF-8");
  f.close();
}

void handle_root() { sendFile("/index.html"); }
void handle_1()    { LED1status = HIGH; sendFile("/on.html"); }
void handle_0()    { LED1status = LOW;  sendFile("/off.html"); }

void setup() {
  Serial.begin(115200);

  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed — zaženi `pio run --target uploadfs`");
  } else {
    Serial.println("LittleFS priklopljen.");
  }

  WiFi.softAP(AP_SSID, AP_PASSWORD);
  delay(100);

  Serial.println("Vzpostavitev esp32 AP - Access Point");
  Serial.print("IP naslov esp32 dostopne točke: ");
  Serial.println(WiFi.softAPIP());

  server.on("/",  handle_root);
  server.on("/1", handle_1);
  server.on("/0", handle_0);

  server.begin();

  pinMode(LED1pin, OUTPUT);
  digitalWrite(LED1pin, HIGH);
  delay(250);
  digitalWrite(LED1pin, LOW);
  delay(250);
  digitalWrite(LED1pin, HIGH);
  delay(250);
  digitalWrite(LED1pin, LOW);
  delay(250);
}

void loop() {
  server.handleClient();

  if (LED1status == HIGH) {
    digitalWrite(LED1pin, HIGH);
  } else {
    digitalWrite(LED1pin, LOW);
  }
}
