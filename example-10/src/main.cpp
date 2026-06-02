#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <LittleFS.h>

#include <secrets.h>
#include <wifi_log.h>

WebServer server(80);

// EN: WebSocket server on port 81. Unlike HTTP (request → response → close),
//     a WebSocket is a **persistent bidirectional** connection: once a
//     client opens it, both sides can send messages instantly without
//     re-establishing anything. Used here for low-latency LED control with
//     no page reloads — the browser sends "1" or "0" over the socket and
//     the ESP32 reacts immediately.
// SL: Strežnik spletnih vtičnikov (WebSocket) na vratih 81. Za razliko od
//     HTTP-ja (zahteva → odgovor → zaprtje) je WebSocket **trajna
//     dvosmerna** povezava: ko jo odjemalec odpre, lahko obe strani takoj
//     pošiljata sporočila brez ponovnega vzpostavljanja. Tu jo uporabljamo
//     za nizkolatentno krmiljenje LED brez osveževanja strani — brskalnik
//     pošlje "1" ali "0" prek vtičnika, ESP32 pa se nemudoma odzove.
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

// EN: WebSocket event callback. The library calls this on every event —
//     client connected/disconnected, text message received, etc. Arguments:
//     `num` is a per-client id (multiple browsers can be connected at once),
//     `type` is the event kind, `payload`/`length` are the message bytes.
// SL: Povratni klic za dogodke spletnega vtičnika. Knjižnica ga pokliče ob
//     vsakem dogodku — priklop/odklop odjemalca, prejeto besedilno
//     sporočilo ipd. Argumenti: `num` je id odjemalca (več brskalnikov je
//     lahko hkrati povezanih), `type` je vrsta dogodka, `payload`/`length`
//     sta vsebina in dolžina sporočila.
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
      // Only the first byte matters here: '1' = on, '0' = off.
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

  // EN: start the WebSocket server on port 81 and bind the event handler.
  //     From this point on, any browser opening `ws://<esp32-ip>:81/` gets
  //     a live bidirectional channel — there are now **two** servers
  //     running (HTTP on 80, WebSocket on 81), each on its own port.
  // SL: zaženemo WebSocket strežnik na vratih 81 in vežemo funkcijo za
  //     dogodke. Od zdaj naprej vsak brskalnik, ki odpre
  //     `ws://<esp32-ip>:81/`, dobi živi dvosmerni kanal — zdaj tečeta
  //     **dva** strežnika (HTTP na 80, WebSocket na 81), vsak na svojih
  //     vratih.
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

  // EN: pump the WebSocket loop — must be called frequently so the server
  //     can deliver incoming events to onWebSocketEvent().
  // SL: poženemo zanko spletnega vtičnika — funkcijo moramo klicati
  //     pogosto, da strežnik dostavi prejete dogodke v onWebSocketEvent().
  webSocket.loop();

  digitalWrite(LED1pin, LED1status ? HIGH : LOW);
}
