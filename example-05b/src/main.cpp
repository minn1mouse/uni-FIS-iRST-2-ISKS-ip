#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include <secrets.h>
#include <wifi_log.h>

WebServer server(80);

// EN: persistent state for HTTP-driven I/O. The /1 and /0 handlers update
//     LED1status; loop() reads it every iteration and drives the pin
//     accordingly. `bool` holds the state (Arduino's HIGH/LOW are 1/0 —
//     they map cleanly to true/false).
// SL: trajno stanje za HTTP-krmiljeni I/O. Funkciji /1 in /0 spreminjata
//     LED1status; loop() ga ob vsaki ponovitvi prebere in glede na to
//     krmili pin. `bool` hrani stanje (Arduino HIGH/LOW sta 1/0 — lepo se
//     preslikata v true/false).
uint8_t LED1pin = 2;
bool LED1status = LOW;

const char HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="sl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>esp32</title>
</head>
<body>
    <h1>Dodaj /1 za vklop LED diode ali /0 za izklop, t. j. /1 ali /0 na koncu URL-ja.</h1>
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
    <h1>Prejet ukaz za vklop LED diode.</h1>
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
    <h1>Prejet ukaz za izklop LED diode.</h1>
</body>
</html>
)rawliteral";

void handle_root() { server.send(200, "text/html; charset=UTF-8", HTML); }

// EN: handler with a side effect — flips LED1status to HIGH, then responds.
//     The HTTP handler stays fast (just sets a flag); the actual GPIO
//     write happens in loop().
// SL: funkcija s stranskim učinkom — LED1status nastavi na HIGH, nato vrne
//     odgovor. Funkcija HTTP ostane hitra (le nastavi zastavico); pravi
//     zapis na GPIO se zgodi v loop().
void handle_1() {
  LED1status = HIGH;
  server.send(200, "text/html; charset=UTF-8", HTML1);
}

void handle_0() {
  LED1status = LOW;
  server.send(200, "text/html; charset=UTF-8", HTML0);
}

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

  // EN: drive the LED pin every iteration based on the stored state. The
  //     decoupling from the HTTP handler is the point — the request returns
  //     quickly, and the hardware update happens in the main loop.
  // SL: ob vsaki ponovitvi krmilimo pin LED glede na shranjeno stanje.
  //     Ločenost od funkcije HTTP je bistvo — zahteva se hitro vrne, zapis
  //     na strojno opremo pa se zgodi v glavni zanki.
  if (LED1status == HIGH) {
    digitalWrite(LED1pin, HIGH);
  } else {
    digitalWrite(LED1pin, LOW);
  }
}

// -----------------------------------------------------------------------------
// EN: Notes / further reading
// SL: Opombe / dodatno branje
// -----------------------------------------------------------------------------

// EN: This is the "hello world" of IoT — an actuator controlled over HTTP.
//     With the same wiring approach you could swap the onboard LED for a
//     relay module on the same pin and switch real-world devices (lamp,
//     fan, etc.). Power the ESP32 from a battery and it becomes a
//     standalone networked device — no laptop needed.
// SL: To je "hello world" interneta stvari (IoT) — aktuator, krmiljen prek
//     HTTP-ja. Z enakim principom bi vgrajeno LED diodo lahko zamenjali z
//     relejskim modulom na isti nožici in vklapljali prave naprave (luč,
//     ventilator itd.). Z napajanjem ESP32 iz baterije postane samostojna
//     omrežna naprava — računalnika ne potrebujemo več.

// EN: Setup vs loop. `setup()` runs once on boot; `loop()` runs continuously
//     as fast as the CPU allows. ESP32's CPU clocks up to 240 MHz, so a
//     trivial `loop()` body iterates millions of times per second — far
//     more than needed to keep HTTP handling responsive.
// SL: setup vs loop. `setup()` se izvede enkrat ob zagonu; `loop()` se
//     izvaja neprestano s polno hitrostjo procesorja. CPU na ESP32 deluje
//     do 240 MHz, zato se preprosto telo `loop()` izvede milijonkrat na
//     sekundo — daleč več, kot potrebujemo za odzivno HTTP obdelavo.

// EN: GPIO = General Purpose Input/Output. The pin driving the onboard LED
//     is an ordinary digital output — it can drive anything that accepts a
//     0 / 3.3 V logic signal. HIGH (1) and LOW (0) map directly to the
//     voltage on the pin.
// SL: GPIO = splošno-namenske vhodno-izhodne nožice. Pin, ki krmili
//     vgrajeno LED, je navaden digitalni izhod — krmili lahko karkoli, kar
//     sprejema logični signal 0 / 3.3 V. HIGH (1) in LOW (0) se
//     preslikata neposredno v napetost na nožici.
