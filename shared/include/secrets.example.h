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

// EN: LAN IP of your laptop / Node-server machine. Same value as in
//     shared/node/config.js. Used by examples that connect OUT (e.g.
//     homework-03). Port is per-example — set inline in each sketch.
// SL: LAN IP tvojega prenosnika / Node strežnika. Enaka vrednost kot v
//     shared/node/config.js. Uporabljajo ga primeri, ki se povezujejo
//     ven (npr. homework-03). Vrata so per-primer — določi jih
//     vrstično v vsaki skici.
#define SERVER_HOST "192.168.0.100"
