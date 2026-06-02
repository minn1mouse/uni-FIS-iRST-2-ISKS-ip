#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include <secrets.h>
#include <wifi_log.h>

WebServer server(80);

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

const char HTML1[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="sl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>esp32</title>
</head>
<body>
    <h1>Spletna stran enojk 111111111111 !</h1>
</body>
</html>
)rawliteral";

const char HTML0[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="sl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>esp32</title>
</head>
<body>
    <h1>Spletna stran ničel 000000000000 !</h1>
</body>
</html>
)rawliteral";

void handle_root() { server.send(200, "text/html; charset=UTF-8", HTML); }
void handle_1()    { server.send(200, "text/html; charset=UTF-8", HTML1); }
void handle_0()    { server.send(200, "text/html; charset=UTF-8", HTML0); }

void setup() {
  Serial.begin(115200);
  printWifiTarget();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Povezovanje z WiFi omrežjem ...");
  }

  Serial.println("Povezava z WiFi omrežjem je vzpostavljena.");
  Serial.print("Moj IP naslov je: ");
  Serial.println(WiFi.localIP());

  // EN: register three routes — each maps a URL path to its own handler.
  //     Open http://<esp32-ip>/   → handle_root (the default greeting)
  //          http://<esp32-ip>/1  → handle_1    (the "ones" page)
  //          http://<esp32-ip>/0  → handle_0    (the "zeros" page)
  //     Anything else returns 404 (handled by WebServer automatically).
  // SL: registriramo tri poti — vsaka URL pot je preslikana na svojo funkcijo.
  //     Odpri  http://<esp32-ip>/   → handle_root (privzeti pozdrav)
  //            http://<esp32-ip>/1  → handle_1    (stran "enojk")
  //            http://<esp32-ip>/0  → handle_0    (stran "ničel")
  //     Karkoli drugega vrne 404 (WebServer ga obravnava samodejno).
  server.on("/",  handle_root);
  server.on("/1", handle_1);
  server.on("/0", handle_0);

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
