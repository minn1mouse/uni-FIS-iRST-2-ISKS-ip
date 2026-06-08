#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

#include <secrets.h>
#include <wifi_log.h>

// EN: seminar-assignment — control system with feedback. The ESP32 is
//     a WebSocket client of the Node hub (port 8811). Browser buttons
//     send `LED` commands which set `greenIntention`; the chip drives
//     the green LED accordingly. A photoresistor on GPIO 32 is read
//     at ~20 Hz and streamed back as `fotoupornik`.
//
//     Feedback loop. The chip compares intention vs LDR:
//       intention=ON  + dark sensor   → "napaka_vklop"  (red reserve on,
//                                                        blue blinks)
//       intention=OFF + bright sensor → "napaka_izklop" (just blue blinks)
//       otherwise → "ok"
//     Status changes are pushed to the hub as `status` messages so the
//     UI can show an error banner. `LDR_THRESHOLD` is the single knob
//     — watch the chart with the LED on (uncovered) vs off, then set
//     it somewhere between the two plateaus.
// SL: seminar-assignment — kontrolni sistem s povratno zanko. ESP32 je
//     WebSocket odjemalec Node zvezdišča (vrata 8811). Gumbi v
//     brskalniku pošljejo ukaz `LED`, ki nastavi `greenIntention`;
//     čip ustrezno krmili zeleno LED. Fotoupornik na GPIO 32 se bere
//     ~20-krat na sekundo in pošilja kot `fotoupornik`.
//
//     Povratna zanka. Čip primerja ukaz in meritev LDR:
//       ukaz=VKLOP  + temno  → "napaka_vklop"  (rezervna rdeča sveti,
//                                                modra utripa)
//       ukaz=IZKLOP + svetlo → "napaka_izklop" (samo modra utripa)
//       sicer → "ok"
//     Spremembe stanja čip pošlje kot sporočilo `status`, da UI prikaže
//     pas z napako. `LDR_THRESHOLD` je edina nastavljiva vrednost — na
//     grafu opazuj vrednost ob prižgani (odkriti) in ob ugasnjeni LED,
//     nato izberi prag nekje med obema.
const uint16_t SERVER_PORT = 8811;

const uint8_t LED_BLUE  = 2;
const uint8_t LED_RED   = 21;
const uint8_t LED_GREEN = 5;
const uint8_t LDR_PIN   = 32;

const int      LDR_THRESHOLD     = 1500;
const uint32_t BLINK_INTERVAL_MS  = 250;
const uint32_t COMMAND_GRACE_MS   = 150;

enum ErrorState { ERR_NONE, ERR_VKLOP, ERR_IZKLOP };

WebSocketsClient webSocket;

bool       greenIntention     = false;
uint32_t   intentionChangedAt = 0;
int        ldrValue           = 0;
ErrorState errorState         = ERR_NONE;
ErrorState lastReported       = ERR_NONE;

uint32_t lastBlinkMs   = 0;
bool     blueBlinkPhase = false;

const char* statusName(ErrorState s) {
  switch (s) {
    case ERR_VKLOP:  return "napaka_vklop";
    case ERR_IZKLOP: return "napaka_izklop";
    default:         return "ok";
  }
}

void sendStatus(ErrorState s) {
  String payload = R"({"tipSporočila":"status","stanje":")";
  payload += statusName(s);
  payload += "\"}";
  webSocket.sendTXT(payload);
}

void onWebSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED:
      Serial.printf("Povezana s strežnikom %s:%u\n", SERVER_HOST, SERVER_PORT);
      // EN: re-announce current status on (re)connect so a freshly
      //     loaded browser tab gets the banner state immediately.
      // SL: ob (ponovni) povezavi ponovno objavimo trenutno stanje, da
      //     sveže naložen zavihek brskalnika takoj dobi stanje pasu.
      lastReported = (ErrorState) -1;
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
        if (pin == LED_GREEN) {
          bool newIntention = (vrednost == 1);
          if (newIntention != greenIntention) {
            greenIntention     = newIntention;
            intentionChangedAt = millis();
          }
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

  pinMode(LED_BLUE,  OUTPUT);
  pinMode(LED_RED,   OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  printWifiTarget();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Povezovanje z WiFi omrežjem ...");
  }

  Serial.println("Modul esp32 je povezan!");
  Serial.print("IP naslov esp32 modula: ");
  Serial.println(WiFi.localIP());

  for (int i = 0; i < 2; i++) {
    digitalWrite(LED_BLUE, HIGH); delay(250);
    digitalWrite(LED_BLUE, LOW);  delay(250);
  }

  webSocket.begin(SERVER_HOST, SERVER_PORT);
  webSocket.onEvent(onWebSocketEvent);
}

void loop() {
  webSocket.loop();

  ldrValue = analogRead(LDR_PIN);

  // EN: comparator — does the sensor agree with the command? `actuallyOn`
  //     is the measurement (bright = LED emitting). Three cases:
  //     intention ↔ measurement match → ok; intention ON but dark →
  //     fail_a (LED removed/burned/covered → light up the reserve);
  //     intention OFF but bright → fail_b (stuck on or external light →
  //     blue blink only, per assignment).
  // SL: komparator — se meritev ujema z ukazom? `actuallyOn` je meritev
  //     (svetlo = LED oddaja). Trije primeri: ukaz in meritev se ujemata
  //     → ok; ukaz VKLOP, a temno → napaka_vklop (LED odstranjena/
  //     pregorela/zakrita → prižgi rezervo); ukaz IZKLOP, a svetlo →
  //     napaka_izklop (zaklenjena ali zunanja svetloba → samo modra
  //     utripa, kot zahteva naloga).
  bool actuallyOn = (ldrValue > LDR_THRESHOLD);

  // EN: grace period after the intention flips — the LED takes a few ms
  //     to physically settle and the ADC needs one more loop iteration
  //     to catch up, so the comparator would otherwise fire one spurious
  //     error per button press. Skip the check for COMMAND_GRACE_MS.
  // SL: čas tolerance po spremembi ukaza — LED potrebuje nekaj ms, da
  //     fizično doseže novo stanje, ADC pa še eno ponovitev zanke, da
  //     to ujame; brez te tolerance bi komparator ob vsakem kliku gumba
  //     sprožil eno lažno napako. Komparator preskočimo za COMMAND_GRACE_MS.
  bool inGrace = (millis() - intentionChangedAt < COMMAND_GRACE_MS);

  if (inGrace) {
    errorState = ERR_NONE;
  } else if (greenIntention && !actuallyOn) {
    errorState = ERR_VKLOP;
  } else if (!greenIntention && actuallyOn) {
    errorState = ERR_IZKLOP;
  } else {
    errorState = ERR_NONE;
  }

  digitalWrite(LED_GREEN, greenIntention ? HIGH : LOW);
  digitalWrite(LED_RED,   (errorState == ERR_VKLOP) ? HIGH : LOW);

  // EN: non-blocking blue blink — only blinks while in error. We can't
  //     use delay() here without stalling the WebSocket loop, so toggle
  //     based on millis(). When ok, force blue off and reset the phase
  //     so the next error starts with a clean LOW edge.
  // SL: neblokirajoča modra utripa — utripa le med napako. delay() tu
  //     ni mogoč, ker bi ustavil WebSocket zanko, zato preklapljamo
  //     glede na millis(). V stanju ok modra je ugasnjena in fazo
  //     ponastavimo, da naslednja napaka začne na LOW.
  if (errorState != ERR_NONE) {
    if (millis() - lastBlinkMs >= BLINK_INTERVAL_MS) {
      lastBlinkMs = millis();
      blueBlinkPhase = !blueBlinkPhase;
      digitalWrite(LED_BLUE, blueBlinkPhase ? HIGH : LOW);
    }
  } else {
    digitalWrite(LED_BLUE, LOW);
    blueBlinkPhase = false;
  }

  // EN: only push a status message when state actually changes — avoids
  //     spamming the hub at 20 Hz. The UI keeps showing the last value.
  // SL: sporočilo `status` pošljemo le ob spremembi stanja — da ne
  //     zasujemo zvezdišča s 20 sporočili na sekundo. UI ohrani zadnjo
  //     prejeto vrednost.
  if (errorState != lastReported) {
    sendStatus(errorState);
    lastReported = errorState;
  }

  String payload = R"({"tipSporočila":"fotoupornik","pin":32,"vrednost":)";
  payload += String(ldrValue);
  payload += "}";
  webSocket.sendTXT(payload);

  delay(50);
}
