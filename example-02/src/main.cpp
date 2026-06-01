#include <Arduino.h>
#include <WiFi.h>

// EN: WiFi credentials come from the repo-wide shared/include/secrets.h
//     (gitignored). It exposes WIFI_SSID and WIFI_PASSWORD, picked between
//     a home network and a hotspot via WIFI_USE_HOTSPOT.
// SL: WiFi gesla so v skupni datoteki shared/include/secrets.h (gitignored).
//     Ta razkriva WIFI_SSID in WIFI_PASSWORD, izbiraš med domačim omrežjem
//     in razrednim hotspotom prek WIFI_USE_HOTSPOT.
#include <secrets.h>
#include <wifi_log.h>

void setup() {
  // EN: setup code — runs once when the device boots.
  // SL: koda za nastavitve — izvede se enkrat, ko se naprava zažene.

  // EN: set the serial communication speed to 115200 bits/s.
  // SL: nastavimo hitrost serijske komunikacije na 115200 bitov/s.
  Serial.begin(115200);

  // EN: log which network slot we're attempting (home vs hotspot) and the SSID.
  // SL: zapišemo, na katero omrežje se poskušamo povezati (home ali hotspot) in SSID.
  printWifiTarget();

  // EN: connect to the WiFi network using SSID and password from secrets.h.
  // SL: povežemo se na WiFi omrežje z imenom in geslom iz secrets.h.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // EN: wait until the module connects to the network.
  // SL: čakamo, dokler se modul ne poveže na omrežje.
  while (WiFi.status() != WL_CONNECTED) {
    // EN: wait 500 ms before re-checking the connection state.
    // SL: počakamo 500 ms preden ponovno preverimo stanje povezave.
    delay(500);
    // EN: print a progress message to the serial monitor.
    // SL: izpišemo sporočilo o napredku na serijski monitor.
    Serial.println("Povezovanje z WiFi omrežjem...");
  }

  // EN: connection established — log success and the assigned IP.
  // SL: povezava vzpostavljena — izpišemo uspeh in dobljeni IP.
  Serial.println("Povezava z WiFi omrežjem je vzpostavljena.");
  Serial.print("Moj IP naslov je: ");
  Serial.println(WiFi.localIP());

  // EN: visual indicator that the program loaded — onboard LED blinks twice.
  // SL: vidni indikator, da se je program naložil — vgrajena LED 2× utripne.
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
  // EN: main code — runs repeatedly while the ESP32 is powered.
  // SL: glavna koda — izvaja se ponavljajoče, dokler je ESP32 vklopljen.
}
