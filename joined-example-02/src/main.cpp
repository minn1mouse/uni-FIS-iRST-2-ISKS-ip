#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

#include <secrets.h>
#include <wifi_log.h>

// EN: ESP32 side of joined-example-02. Same WebSocket-client topology as
//     joined-example-01 (connects to the Node hub on SERVER_HOST:8811),
//     but now the chip **parses the incoming JSON** with ArduinoJson
//     instead of just looking at `payload[0]`. The closed loop now works
//     end-to-end with the lecturer's default JSON value:
//       {"tipSporočila":"LED","pin":2,"vrednost":1}
//     Same wire format as homework-03 — the difference is the messages
//     originate from a browser via the Node hub, not from a script the
//     server runs itself.
// SL: Stran ESP32 v joined-example-02. Enaka WebSocket-odjemalska
//     topologija kot v joined-example-01 (povezava na Node zvezdišče
//     SERVER_HOST:8811), vendar čip zdaj **razčleni JSON** z
//     ArduinoJson namesto, da bi gledal le `payload[0]`. Krog se zdaj
//     sklene od konca do konca s privzeto JSON vrednostjo:
//       {"tipSporočila":"LED","pin":2,"vrednost":1}
//     Enaka oblika sporočila kot v homework-03 — razlika je, da
//     sporočila prihajajo iz brskalnika prek Node zvezdišča, ne iz
//     skripte, ki teče v strežniku.
const uint16_t SERVER_PORT = 8811;

WebSocketsClient webSocket;

uint8_t LED1pin    = 2;
bool    LED1status = LOW;

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
        } else {
          Serial.printf("Pin %d ni podprt (samo pin %d).\n", pin, LED1pin);
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
  digitalWrite(LED1pin, HIGH); delay(250);
  digitalWrite(LED1pin, LOW);  delay(250);
  digitalWrite(LED1pin, HIGH); delay(250);
  digitalWrite(LED1pin, LOW);  delay(250);

  webSocket.begin(SERVER_HOST, SERVER_PORT);
  webSocket.onEvent(onWebSocketEvent);
}

void loop() {
  webSocket.loop();
  digitalWrite(LED1pin, LED1status ? HIGH : LOW);
}
