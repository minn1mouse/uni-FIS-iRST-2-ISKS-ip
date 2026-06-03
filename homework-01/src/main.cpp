#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>

#include <secrets.h>
#include <wifi_log.h>

WebServer server(80);

// EN: three LEDs on three pins — onboard blue + two external (red on GPIO 5,
//     green on GPIO 18). Each has its own boolean state and its own on/off
//     route pair (see server.on(...) calls in setup).
// SL: tri LED diode na treh nožicah — vgrajena modra + dve zunanji (rdeča
//     na GPIO 5, zelena na GPIO 18). Vsaka ima svoje bool stanje in svoj
//     par poti za vklop/izklop (glej server.on(...) klice v setup).
const uint8_t LED_BLUE  = 2;
const uint8_t LED_RED   = 5;
const uint8_t LED_GREEN = 18;

bool blueOn  = false;
bool redOn   = false;
bool greenOn = false;

// EN: read the HTML template from LittleFS, substitute placeholders
//     (%BLUE%, %BLUE_CLASS%, etc.) with the current state, and send. This
//     is a minimal server-side templating pattern using String::replace —
//     enough for a handful of dynamic values. For larger sites you'd reach
//     for a real templating library or send raw state as JSON and render
//     client-side with JavaScript.
// SL: preberi HTML predlogo iz LittleFS, zamenjaj oznake (%BLUE%,
//     %BLUE_CLASS% ipd.) s trenutnim stanjem in pošlji. Preprost vzorec
//     strežniške predloge z String::replace — dovolj za nekaj dinamičnih
//     vrednosti. Za večja mesta uporabi pravo knjižnico za predloge ali
//     pa stanje pošlji kot JSON in renderiraj na odjemalcu z JavaScript-om.
void sendPage() {
  File f = LittleFS.open("/index.html", "r");
  if (!f) {
    server.send(500, "text/plain; charset=UTF-8",
                "Manjka /index.html — zaženi `pio run --target uploadfs`");
    return;
  }
  String page = f.readString();
  f.close();

  page.replace("%BLUE%",        blueOn  ? "PRIŽGANA" : "UGASNJENA");
  page.replace("%RED%",         redOn   ? "PRIŽGANA" : "UGASNJENA");
  page.replace("%GREEN%",       greenOn ? "PRIŽGANA" : "UGASNJENA");
  page.replace("%BLUE_CLASS%",  blueOn  ? "is-on" : "is-off");
  page.replace("%RED_CLASS%",   redOn   ? "is-on" : "is-off");
  page.replace("%GREEN_CLASS%", greenOn ? "is-on" : "is-off");

  server.send(200, "text/html; charset=UTF-8", page);
}

void handle_root() { sendPage(); }
void handle_0()    { blueOn  = false; sendPage(); }
void handle_1()    { blueOn  = true;  sendPage(); }
void handle_2()    { redOn   = false; sendPage(); }
void handle_3()    { redOn   = true;  sendPage(); }
void handle_4()    { greenOn = false; sendPage(); }
void handle_5()    { greenOn = true;  sendPage(); }

void setup() {
  Serial.begin(115200);

  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed — zaženi `pio run --target uploadfs`");
  } else {
    Serial.println("LittleFS priklopljen.");
  }

  pinMode(LED_BLUE,  OUTPUT);
  pinMode(LED_RED,   OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

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
  server.on("/0", handle_0);  // blue OFF
  server.on("/1", handle_1);  // blue ON
  server.on("/2", handle_2);  // red OFF
  server.on("/3", handle_3);  // red ON
  server.on("/4", handle_4);  // green OFF
  server.on("/5", handle_5);  // green ON

  server.begin();

  for (int i = 0; i < 2; i++) {
    digitalWrite(LED_BLUE, HIGH); delay(250);
    digitalWrite(LED_BLUE, LOW);  delay(250);
  }
}

void loop() {
  server.handleClient();

  digitalWrite(LED_BLUE,  blueOn  ? HIGH : LOW);
  digitalWrite(LED_RED,   redOn   ? HIGH : LOW);
  digitalWrite(LED_GREEN, greenOn ? HIGH : LOW);
}
