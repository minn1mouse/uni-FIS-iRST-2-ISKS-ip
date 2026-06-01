#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include <secrets.h>
#include <wifi_log.h>

// EN: WebServer object listening on port 80 (the default HTTP port).
// SL: Objekt WebServer, ki posluša na vratih 80 (privzeta vrata za HTTP).
WebServer server(80);

// EN: HTTP route handler for the root path "/". The server calls this
//     function when a client (e.g. Chrome) requests http://<esp32-ip>/.
//     We respond with a simple HTML greeting and status 200 (OK).
// SL: Funkcija za obravnavo zahteve na korenski poti "/". Strežnik jo
//     pokliče, ko odjemalec (npr. Chrome) zahteva http://<esp32-ip>/.
//     Odgovorimo s preprostim HTML pozdravom in statusom 200 (OK).
void handle_root() {
  server.send(200, "text/html; charset=UTF-8", "Pozdravljen svet iz esp32! - Minnie");
}

void setup() {
  Serial.begin(115200);
  printWifiTarget();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Povezovanje z WiFi omrežjem...");
  }

  Serial.println("Povezava z WiFi omrežjem je vzpostavljena.");
  Serial.print("Moj IP naslov je: ");
  Serial.println(WiFi.localIP());

  // EN: register the route — when a client requests "/", call handle_root().
  // SL: registriramo pot — ko odjemalec zahteva "/", se pokliče handle_root().
  server.on("/", handle_root);

  // EN: start the HTTP server — it now listens on port 80 for client requests.
  // SL: zaženemo HTTP strežnik — odslej posluša na vratih 80 in čaka zahteve.
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
  // EN: handle pending HTTP requests — must be called frequently so the
  //     server can accept connections and dispatch them to the registered
  //     handlers (e.g. handle_root).
  // SL: obdelamo morebitne čakajoče HTTP zahteve — funkcijo moramo klicati
  //     pogosto, da strežnik sprejme povezave in jih preda registriranim
  //     funkcijam (npr. handle_root).
  server.handleClient();
}

// -----------------------------------------------------------------------------
// EN: Notes / further reading
// SL: Opombe / dodatno branje
// -----------------------------------------------------------------------------

// EN: This is essentially how a router's admin UI works too. With very little
//     code we've built a web server: visit the printed IP in a browser and
//     the ESP32 returns "Pozdravljen svet…" — same mechanism a router uses
//     to serve its settings page. The client (computer/phone) and the ESP32
//     must be on the same WiFi for the IP to be reachable; the printed IP
//     is the server's.
// SL: Tako v bistvu deluje tudi skrbniški vmesnik usmerjevalnika. S kratko
//     kodo smo postavili spletni strežnik: vpiši izpisani IP v brskalnik in
//     ESP32 vrne "Pozdravljen svet…" — enak postopek uporablja usmerjevalnik
//     za prikaz strani z nastavitvami. Odjemalec (računalnik/telefon) in
//     ESP32 morata biti v istem WiFi-ju, da je IP dosegljiv; izpisani IP je
//     strežnikov.

// EN: ESP32 memory. The chip has ~300–500 KB of RAM available for the sketch
//     and 4 MB of flash — enough for a couple of megabytes of static content.
//     If you need more (and don't expect many concurrent clients), add an SD
//     card via a breakout board and storage is effectively unlimited.
// SL: Pomnilnik ESP32. Čip ima ~300–500 KB pomnilnika (RAM) za skico in
//     4 MB flash pomnilnika — dovolj za nekaj megabajtov statične vsebine.
//     Če potrebuješ več (in ne pričakuješ veliko sočasnih odjemalcev),
//     priklopi SD kartico prek prototipne ploščice in pomnilnika je
//     praktično poljubno veliko.

// EN: Energy comparison. A Raspberry Pi makes a more capable home web server
//     at ~5 W of draw — less than a classic incandescent bulb. Compare that
//     to four hob rings on a kitchen stove drawing several kilowatts: one
//     dinner of stovetop power would run a Raspberry Pi for about ten years.
// SL: Primerjava porabe energije. Raspberry Pi je bolj zmogljiv domači
//     spletni strežnik s porabo ~5 W — manj kot klasična žarnica. V
//     primerjavi: štiri obroči kuhalne plošče porabijo nekaj kilovatov; en
//     obrok kuhanja bi torej napajal Raspberry Pi približno deset let.

// EN: Sensors. Just like an SD card, you can attach sensors to the ESP32 —
//     typically two pins for power and two for data. Examples: temperature,
//     distance, humidity sensors, or a photoresistor (LDR) — there's one in
//     our kit.
// SL: Tipala. Podobno kot SD kartico lahko na ESP32 priklopiš tipala —
//     običajno dve nožici za napajanje in dve za podatke. Primeri: tipala
//     temperature, razdalje, vlage ali fotoupornik (LDR) — enega imamo v
//     kompletu.
