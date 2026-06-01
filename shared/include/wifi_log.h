#pragma once

#include <Arduino.h>
#include <secrets.h>

// EN: Prints which credential slot the current build is using (home or
//     hotspot) and the SSID. Call after Serial.begin() and before
//     WiFi.begin() so the user sees what the chip is about to attempt.
// SL: Izpiše, katero gnezdo gesel uporablja trenutna gradnja (home ali
//     hotspot) in SSID. Pokliči po Serial.begin() in pred WiFi.begin(),
//     da uporabnik vidi, na kaj se čip poskuša povezati.
inline void printWifiTarget() {
  Serial.printf("Povezovanje na %s omrežje: %s\n", WIFI_NETWORK_LABEL, WIFI_SSID);
}
