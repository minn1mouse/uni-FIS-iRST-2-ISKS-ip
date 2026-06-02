#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include <secrets.h>
#include <wifi_log.h>

// EN: LittleFS = a small filesystem stored in the ESP32's flash. Files in
//     this project's data/ folder are written there by
//     `pio run --target uploadfs`. The C++ code below reads them at
//     runtime instead of carrying inline PROGMEM strings.
// SL: LittleFS = majhen datotečni sistem v flash pomnilniku ESP32. Datoteke
//     iz mape data/ tega projekta tja zapiše ukaz
//     `pio run --target uploadfs`. Spodnja koda jih bere v času izvajanja,
//     namesto da bi nosila vsebino v PROGMEM nizih.
#include <LittleFS.h>

WebServer server(80);

uint8_t LED1pin = 2;
bool LED1status = LOW;

// EN: read a file from LittleFS and stream it as the HTTP response.
//     Returns 404 if the path is missing — useful diagnostic if you
//     forgot the `uploadfs` step.
// SL: preberi datoteko iz LittleFS in jo pošlji kot HTTP odgovor.
//     Vrne 404, če pot ne obstaja — koristna diagnostika, če si
//     pozabila zagnati `uploadfs`.
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

  // EN: mount the LittleFS partition. If this fails, the data/ folder
  //     was never uploaded — run `pio run --target uploadfs`. Continue
  //     anyway so WiFi info still prints; route handlers will return 404.
  // SL: priklopi LittleFS particijo. Če ne uspe, mapa data/ ni bila
  //     naložena — zaženi `pio run --target uploadfs`. Nadaljujemo, da se
  //     WiFi podatki še izpišejo; funkcije vrnejo 404.
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed — zaženi `pio run --target uploadfs`");
  } else {
    Serial.println("LittleFS priklopljen.");
  }

  printWifiTarget();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Povezovanje z WiFi omrežjem ...");
  }

  Serial.println("Povezava z WiFi omrežjem je vzpostavljena.");
  Serial.print("Moj IP naslov je: ");
  Serial.println(WiFi.localIP());

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
