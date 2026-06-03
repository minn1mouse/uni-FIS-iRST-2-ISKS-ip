#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

#include <secrets.h>
#include <wifi_log.h>

// EN: ESP32 side of joined-example-03 — **bidirectional**. Same JSON
//     receive path as joined-example-02 (LED commands from the browser
//     via Node), plus a new outbound path: a physical button on GPIO 18
//     is read every loop; when its state changes, the chip pushes a
//     `{"tipSporočila":"tipka","pin":18,"vrednost":0|1}` message back
//     through the same socket. Node routes it to all browsers, which
//     display the latest value.
// SL: Stran ESP32 v joined-example-03 — **dvosmerna**. Enak sprejem
//     JSON sporočil kot v joined-example-02 (ukazi za LED iz brskalnika
//     prek Node-a), poleg tega pa nova pot navzven: fizična tipka na
//     GPIO 18 se bere v vsaki zanki; ob spremembi stanja čip prek istega
//     vtičnika pošlje sporočilo
//     `{"tipSporočila":"tipka","pin":18,"vrednost":0|1}`. Node ga
//     usmeri vsem brskalnikom, ki prikažejo zadnjo vrednost.
const uint16_t SERVER_PORT = 8811;
const uint8_t  BUTTON_PIN  = 18;

WebSocketsClient webSocket;

uint8_t LED1pin    = 2;
bool    LED1status = LOW;

int prejsnjeStanje = LOW;
int trenutnoStanje;

void onWebSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED:
      Serial.printf("Povezana s strežnikom %s:%u\n", SERVER_HOST, SERVER_PORT);
      break;

    case WStype_DISCONNECTED:
      Serial.println("Povezava s strežnikom prekinjena.");
      break;

    case WStype_TEXT: {
      Serial.printf("Prejeto: %s\n", payload);

      StaticJsonDocument<256> doc;
      DeserializationError err = deserializeJson(doc, payload);
      if (err) {
        Serial.print("deserializeJson() — napaka: ");
        Serial.println(err.c_str());
        return;
      }

      const char* tip = doc["tipSporočila"];
      if (tip && strcmp(tip, "LED") == 0) {
        int pin      = doc["pin"];
        int vrednost = doc["vrednost"];
        if (pin == LED1pin) {
          LED1status = (vrednost == 1) ? HIGH : LOW;
        }
      }
      break;
    }

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

  pinMode(LED1pin, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  digitalWrite(LED1pin, HIGH); delay(250);
  digitalWrite(LED1pin, LOW);  delay(250);
  digitalWrite(LED1pin, HIGH); delay(250);
  digitalWrite(LED1pin, LOW);  delay(250);

  webSocket.begin(SERVER_HOST, SERVER_PORT);
  webSocket.onEvent(onWebSocketEvent);
}

void loop() {
  webSocket.loop();

  // EN: drive the LED from the state set by incoming messages.
  // SL: krmilimo LED glede na stanje, ki ga je nastavilo dohodno sporočilo.
  digitalWrite(LED1pin, LED1status ? HIGH : LOW);

  // EN: edge-detect the button — only send a message when the state
  //     CHANGES, not every loop iteration. Uses a raw-string literal so
  //     the JSON's double quotes don't need escaping. The 50 ms loop
  //     delay below acts as crude debouncing.
  // SL: zaznava roba na tipki — sporočilo pošljemo le ob SPREMEMBI
  //     stanja, ne ob vsaki ponovitvi zanke. Uporabimo surov niz, tako
  //     da dvojnih narekovajev v JSON-u ni treba ubežati. 50 ms
  //     zakasnitev v zanki služi kot groba debouncing zaščita.
  trenutnoStanje = digitalRead(BUTTON_PIN);
  if (trenutnoStanje != prejsnjeStanje) {
    String dataString = (trenutnoStanje == HIGH)
        ? R"({"tipSporočila":"tipka","pin":18,"vrednost":1})"
        : R"({"tipSporočila":"tipka","pin":18,"vrednost":0})";
    webSocket.sendTXT(dataString);
    prejsnjeStanje = trenutnoStanje;
  }

  delay(50);
}
