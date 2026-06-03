#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>

#include <secrets.h>
#include <wifi_log.h>

// EN: ESP32 side of joined-example-04a — identical to joined-example-04.
//     Same potentiometer-streaming logic: read GPIO 34 ADC ~20 Hz, push
//     `{"tipSporočila":"potenciometer","pin":34,"vrednost":<int>}` to
//     the Node hub. The **only** difference vs joined-04 is on the
//     browser side: the page now plots the incoming values as a live
//     line graph on an HTML5 canvas instead of just showing the latest
//     number. No chip-side change.
// SL: Stran ESP32 v joined-example-04a — identično kot joined-example-04.
//     Enako pošiljanje meritev potenciometra: branje ADC-ja GPIO 34 z
//     ~20 Hz, pošiljanje JSON sporočila
//     `{"tipSporočila":"potenciometer","pin":34,"vrednost":<int>}` na
//     Node zvezdišče. **Edina** razlika v primerjavi z joined-04 je na
//     strani brskalnika: stran dohodne vrednosti zdaj izrisuje kot
//     živi linijski graf na HTML5 platnu, namesto da bi prikazovala le
//     zadnjo številko. Na čipu se ne spremeni nič.
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
