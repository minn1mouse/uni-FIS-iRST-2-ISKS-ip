#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <LittleFS.h>

#include <secrets.h>
#include <wifi_log.h>

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

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

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED: {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Klient povezan z IP: %d.%d.%d.%d\n",
                    num, ip[0], ip[1], ip[2], ip[3]);
      break;
    }
    case WStype_DISCONNECTED: {
      Serial.printf("[%u] Klient odklopljen!\n", num);
      break;
    }
    case WStype_TEXT: {
      Serial.printf("[%u] Prejeto sporočilo: %s\n", num, payload);
      if (payload[0] == '1') {
        LED1status = HIGH;
      } else if (payload[0] == '0') {
        LED1status = LOW;
      }
      break;
    }
    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);

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

  server.on("/", handle_root);
  server.begin();
  Serial.println("HTTP strežnik je zagnan na vratih 80.");

  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
  Serial.println("WebSocket strežnik je zagnan na vratih 81.");

  pinMode(LED1pin, OUTPUT);
  digitalWrite(LED1pin, HIGH); delay(250);
  digitalWrite(LED1pin, LOW);  delay(250);
  digitalWrite(LED1pin, HIGH); delay(250);
  digitalWrite(LED1pin, LOW);  delay(250);
}

void loop() {
  server.handleClient();
  webSocket.loop();

  digitalWrite(LED1pin, LED1status ? HIGH : LOW);
}
