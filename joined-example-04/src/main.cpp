#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>

#include <secrets.h>
#include <wifi_log.h>

// EN: ESP32 side of joined-example-04 — **live sensor streaming**.
//     Reads an analog potentiometer on GPIO 34 (`analogRead` returns
//     0–4095, the 12-bit ADC value), then ~20 times per second pushes
//     the reading to the Node hub as a JSON message of shape
//     `{"tipSporočila":"potenciometer","pin":34,"vrednost":<int>}`.
//     Node forwards it to all browsers, which display the value.
//     The same chip-only "just read + print to serial" sketch lives at
//     example-12 — this example wires that sensor output up to a Node
//     hub instead of the serial monitor.
// SL: Stran ESP32 v joined-example-04 — **živi tok meritev senzorja**.
//     Bere analogni potenciometer na GPIO 34 (`analogRead` vrne
//     0–4095, 12-bitna vrednost ADC-ja), nato pa približno 20-krat na
//     sekundo pošlje meritev na Node zvezdišče kot JSON sporočilo
//     oblike `{"tipSporočila":"potenciometer","pin":34,"vrednost":<int>}`.
//     Node ga posreduje vsem brskalnikom, ki vrednost prikažejo.
//     Ista, samo-čipovska "preberi + izpiši na serijski" skica živi v
//     example-12 — ta primer izhod senzorja namesto na serijski monitor
//     usmeri na Node zvezdišče.
const uint16_t SERVER_PORT = 8811;
const uint8_t  POT_PIN     = 34;

WebSocketsClient webSocket;

int vrednostPotenciometra = 0;

void onWebSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED:
      Serial.printf("Povezana s strežnikom %s:%u\n", SERVER_HOST, SERVER_PORT);
      break;
    case WStype_DISCONNECTED:
      Serial.println("Povezava s strežnikom prekinjena.");
      break;
    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);

  printWifiTarget();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Povezovanje z WiFi omrežjem ...");
  }

  Serial.println("Modul esp32 je povezan!");
  Serial.print("IP naslov esp32 modula: ");
  Serial.println(WiFi.localIP());

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH); delay(250);
  digitalWrite(2, LOW);  delay(250);
  digitalWrite(2, HIGH); delay(250);
  digitalWrite(2, LOW);  delay(250);

  webSocket.begin(SERVER_HOST, SERVER_PORT);
  webSocket.onEvent(onWebSocketEvent);
}

void loop() {
  webSocket.loop();

  // EN: read the ADC, format a JSON payload, send. Inline raw-string
  //     literal + `String + …` concatenation: simplest way to inject a
  //     dynamic integer into JSON without ArduinoJson. The chip is a
  //     producer here, so we never need to parse anything coming in.
  // SL: preberi ADC, sestavi JSON, pošlji. Surov niz + `String + …`
  //     združevanje: najpreprostejši način, da dinamično vrednost
  //     vstaviš v JSON brez ArduinoJson-a. Čip je tu proizvajalec, zato
  //     nam dohodnih sporočil ni treba razčlenjevati.
  vrednostPotenciometra = analogRead(POT_PIN);

  String dataString = R"({"tipSporočila":"potenciometer","pin":34,"vrednost":)";
  dataString += String(vrednostPotenciometra);
  dataString += "}";

  webSocket.sendTXT(dataString);

  delay(50); // ~20 Hz stream
}
