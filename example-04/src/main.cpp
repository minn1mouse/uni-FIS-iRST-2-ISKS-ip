#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include <secrets.h>
#include <wifi_log.h>

WebServer server(80);

// EN: HTML page stored as a const char[] in PROGMEM (flash memory, not RAM)
//     so the big string doesn't eat into the limited RAM. The
//     R"rawliteral( ... )rawliteral" wrapper is a C++11 raw string literal —
//     lets us write multi-line HTML without escaping quotes or newlines.
//     `charset="UTF-8"` and `lang="sl"` on the document make Slovene letters
//     (čšž) render correctly in the browser.
// SL: HTML stran shranimo kot const char[] v PROGMEM (flash pomnilnik, ne
//     RAM), da velik niz ne porabi dragocenega RAM-a. Zapis
//     R"rawliteral( ... )rawliteral" je C++11 surovi niz (raw string literal)
//     — omogoča pisanje večvrstičnega HTML-ja brez ubežnih znakov.
//     `charset="UTF-8"` in `lang="sl"` v dokumentu poskrbita, da se slovenske
//     črke (čšž) v brskalniku pravilno prikažejo.
const char HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="sl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>esp32</title>
</head>
<body>
    <h1>Pozdravljen svet iz esp32, čšž-ji delujejo!</h1>
</body>
</html>
)rawliteral";

void handle_root() {
  server.send(200, "text/html; charset=UTF-8", HTML);
}

void setup() {
  Serial.begin(115200);
  printWifiTarget();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Povezovanje z WiFi omrežjem...");
  }

  Serial.println("Povezava z WiFi omrežjem je vzpostavljena.");
  Serial.print("IP naslov esp32 modula je: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_root);
  server.begin();

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  delay(250);
  digitalWrite(2, LOW);
  delay(250);
  digitalWrite(2, HIGH);
  delay(250);
  digitalWrite(2, LOW);
  delay(250);
}

void loop() {
  server.handleClient();
}
 