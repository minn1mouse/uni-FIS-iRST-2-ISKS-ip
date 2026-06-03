#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>

#include <secrets.h>
#include <wifi_log.h>

// EN: WebSocket client that connects to the Node "hub" server running on
//     the LAN (see ../node-example-08/). The Node side forwards every
//     message the browser sends down this chip's socket; here we just
//     look at the first byte — '1' turns the LED on, '0' off. Same
//     simple wire format as ESP32 example-11, just routed through Node
//     instead of received directly.
// SL: WebSocket odjemalec, ki se povezuje na Node "hub" strežnik v
//     lokalnem omrežju (glej ../node-example-08/). Node posreduje vsako
//     sporočilo brskalnika v vtičnik tega čipa; tu gledamo le prvi bajt
//     — '1' prižge LED, '0' ugasne. Enaka preprosta oblika kot v ESP32
//     example-11, le da gre tu prek Node-a in ne neposredno.
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

    case WStype_TEXT:
      Serial.printf("Prejeto: %s\n", payload);
      if      (payload[0] == '1') LED1status = HIGH;
      else if (payload[0] == '0') LED1status = LOW;
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
