#pragma once

// EN: WiFi credential template — copy this file to `secrets.h` (next to it)
//     and fill in your real values. `secrets.h` is gitignored and will never
//     be committed.
// SL: Predloga za WiFi gesla — to datoteko kopiraj v `secrets.h` (poleg te)
//     in vpiši prave podatke. `secrets.h` je v .gitignore in se nikoli ne
//     objavi na Git.

// EN: Home network
// SL: Domače omrežje
#define WIFI_HOME_SSID     "your-home-ssid"
#define WIFI_HOME_PASSWORD "your-home-password"

// EN: Classroom hotspot
// SL: Razredni hotspot
#define WIFI_HOTSPOT_SSID     "your-classroom-ssid"
#define WIFI_HOTSPOT_PASSWORD "your-classroom-password"

// EN: Active network: 0 = home, 1 = hotspot.
// SL: Aktivno omrežje: 0 = domače, 1 = hotspot.
#define WIFI_USE_HOTSPOT 0

#if WIFI_USE_HOTSPOT
  #define WIFI_SSID          WIFI_HOTSPOT_SSID
  #define WIFI_PASSWORD      WIFI_HOTSPOT_PASSWORD
  #define WIFI_NETWORK_LABEL "hotspot"
#else
  #define WIFI_SSID          WIFI_HOME_SSID
  #define WIFI_PASSWORD      WIFI_HOME_PASSWORD
  #define WIFI_NETWORK_LABEL "home"
#endif
