#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>

WebServer server(80);

// EN: AP (access point) credentials — the SSID/password that clients
//     (laptops, phones) use to JOIN the network the ESP32 itself
//     broadcasts. These are not secrets in the usual sense (the SSID is
//     public the moment the AP turns on), so they don't belong in
//     shared/include/secrets.h. That header is for credentials of remote
//     networks we connect TO, not networks we host.
// SL: AP (access point) podatki — SSID/geslo, ki ju odjemalci (prenosniki,
//     telefoni) uporabijo, da se PRIDRUŽIJO omrežju, ki ga ESP32 sam
//     oddaja. Nista skrivnost v običajnem smislu (SSID je javen v
//     trenutku, ko se AP vklopi), zato ne sodita v
//     shared/include/secrets.h. Ta datoteka je za podatke oddaljenih
//     omrežij, na katera se POVEZUJEMO, ne za omrežja, ki jih gostimo.
const char* AP_SSID     = "esp32_1";
const char* AP_PASSWORD = "12345678";

void handle_root() {
  File f = LittleFS.open("/index.html", "r");
  if (!f) {
    server.send(404, "text/plain; charset=UTF-8", "Manjka /index.html");
    return;
  }
  server.streamFile(f, "text/html; charset=UTF-8");
  f.close();
}

void setup() {
  Serial.begin(115200);

  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed — zaženi `pio run --target uploadfs`");
  } else {
    Serial.println("LittleFS priklopljen.");
  }

  // EN: start a software access point. Instead of WiFi.begin() (which
  //     joins an existing network), WiFi.softAP() makes the ESP32
  //     broadcast its own WiFi network. No router or internet is needed
  //     — everything is local. Clients reach the server at the default
  //     soft-AP IP, 192.168.4.1.
  // SL: zaženemo programsko dostopno točko. Namesto WiFi.begin() (ki se
  //     pridruži obstoječemu omrežju) WiFi.softAP() povzroči, da ESP32
  //     sam oddaja WiFi omrežje. Usmerjevalnik ali internet nista
  //     potrebna — vse je lokalno. Odjemalci dosežejo strežnik na
  //     privzetem soft-AP IP-ju 192.168.4.1.
  WiFi.softAP(AP_SSID, AP_PASSWORD);

  Serial.println("Vzpostavitev programske dostopne točke na esp32 modulu.");
  Serial.print("IP naslov esp32 dostopne točke: ");
  Serial.println(WiFi.softAPIP());

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
