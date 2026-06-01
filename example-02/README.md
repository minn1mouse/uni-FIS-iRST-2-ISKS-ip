# Example 02 — Connect ESP32 to WiFi

The second example. The ESP32 connects to a 2.4 GHz WiFi network using credentials kept out of the source tree (`shared/include/secrets.h`, gitignored), then prints its assigned IP and double-blinks the onboard LED to signal "program loaded". Two networks are pre-wired in the secrets header — **home** and **hotspot** — and you switch between them by editing a single `#define`.

Drugi primer. ESP32 se poveže na 2.4 GHz WiFi omrežje z uporabo gesel, ki niso del izvorne kode (`shared/include/secrets.h`, gitignored), nato izpiše dobljeni IP naslov in 2× utripne z vgrajeno LED kot znak, da je program naložen. V skupni datoteki gesel sta vnaprej pripravljeni dve omrežji — **home** in **hotspot** — med katerima preklopiš s spremembo enega `#define`-a.

---

## English

### Hardware

- ESP32 development board (configured as `esp32dev` in `platformio.ini`)
- USB cable for power and flashing
- A reachable **2.4 GHz** WiFi network or phone hotspot (the ESP32 does not support 5 GHz — for phone hotspots enable "Maximize Compatibility" on iOS or "2.4 GHz / Compatibility mode" on Android)

No external wiring required. Onboard LED on **GPIO 2** is used as the visual indicator.

### One-time configuration

Before building, set up your credentials:

1. Copy [shared/include/secrets.example.h](../shared/include/secrets.example.h) to `shared/include/secrets.h` (same folder).
2. Open `secrets.h` and fill in real values:
   - `WIFI_HOME_SSID` / `WIFI_HOME_PASSWORD` — home network
   - `WIFI_HOTSPOT_SSID` / `WIFI_HOTSPOT_PASSWORD` — classroom / phone hotspot
3. Choose which network to use for this build by setting `WIFI_USE_HOTSPOT` to `0` (home) or `1` (hotspot).

`secrets.h` is listed in the repo's root `.gitignore` and will not be committed.

### How it works

- `Serial.begin(115200)` — open the serial console.
- `printWifiTarget()` (from `wifi_log.h`) — print which slot (`home` / `hotspot`) and SSID the build is about to attempt.
- `WiFi.begin(WIFI_SSID, WIFI_PASSWORD)` — start the connection.
- Loop on `WiFi.status() != WL_CONNECTED` with a 500 ms delay, printing progress dots, until connected.
- Print success line and `WiFi.localIP()`.
- Double-blink the onboard LED on GPIO 2 to mark "program loaded".
- `loop()` is empty — the example is connect-once-and-stop.

### Build, upload, monitor

Run from inside `example-02/`:

```bash
pio run                    # compile
pio run --target upload    # flash to the connected ESP32
pio device monitor         # open serial monitor (115200 baud)
```

### Expected serial output

```
Povezovanje na hotspot omrežje: <your-ssid>
Povezovanje z WiFi omrežjem...
Povezovanje z WiFi omrežjem...
...
Povezava z WiFi omrežjem je vzpostavljena.
Moj IP naslov je: 192.168.x.x
```

…followed by the onboard LED blinking twice.

---

## Slovensko

### Strojna oprema

- Razvojna ploščica ESP32 (nastavljena kot `esp32dev` v `platformio.ini`)
- USB kabel za napajanje in nalaganje
- Dosegljivo **2.4 GHz** WiFi omrežje ali telefonski hotspot (ESP32 ne podpira 5 GHz — na iPhonu vklopi "Maximize Compatibility", na Androidu "2.4 GHz / Compatibility mode")

Dodatno ožičenje ni potrebno. Vgrajena LED na **GPIO 2** služi kot vidni indikator.

### Enkratna nastavitev

Pred prevajanjem nastavi gesla:

1. Kopiraj [shared/include/secrets.example.h](../shared/include/secrets.example.h) v `shared/include/secrets.h` (ista mapa).
2. Odpri `secrets.h` in vpiši prave vrednosti:
   - `WIFI_HOME_SSID` / `WIFI_HOME_PASSWORD` — domače omrežje
   - `WIFI_HOTSPOT_SSID` / `WIFI_HOTSPOT_PASSWORD` — hotspot v razredu ali na telefonu
3. Izberi aktivno omrežje za to gradnjo z `WIFI_USE_HOTSPOT`: `0` (home) ali `1` (hotspot).

`secrets.h` je naveden v korenskem `.gitignore` in se ne objavi.

### Kako deluje

- `Serial.begin(115200)` — odpremo serijsko konzolo.
- `printWifiTarget()` (iz `wifi_log.h`) — izpišemo, na katero gnezdo (`home` / `hotspot`) in SSID se gradnja poskuša povezati.
- `WiFi.begin(WIFI_SSID, WIFI_PASSWORD)` — sproži povezavo.
- Zanka preverja `WiFi.status() != WL_CONNECTED` z zakasnitvijo 500 ms in izpisom napredka, dokler ni povezave.
- Izpis uspeha in `WiFi.localIP()`.
- Vgrajena LED na GPIO 2 2× utripne kot znak "program naložen".
- `loop()` je prazen — primer se enkrat poveže in obstane.

### Prevajanje, nalaganje, spremljanje

Vse ukaze poganjajte iz mape `example-02/`:

```bash
pio run                    # prevedi
pio run --target upload    # naloži na priključen ESP32
pio device monitor         # odpri serijski monitor (115200 baud)
```

### Pričakovani serijski izpis

```
Povezovanje na hotspot omrežje: <tvoj-ssid>
Povezovanje z WiFi omrežjem...
Povezovanje z WiFi omrežjem...
...
Povezava z WiFi omrežjem je vzpostavljena.
Moj IP naslov je: 192.168.x.x
```

…sledi 2× utrip vgrajene LED.
