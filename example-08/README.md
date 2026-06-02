# Example 08 — Control the LED over HTTP, URL-only (AP mode)

Combines [example-07](../example-07/README.md) (soft access point) with the [example-05c](../example-05c/README.md) LED state machine. **No clickable UI yet** — the user navigates by typing `/1` or `/0` at the end of the URL in the browser's address bar. example-09 adds clickable buttons on top of the same code.

Združuje [example-07](../example-07/README.md) (programska dostopna točka) in stroj stanj iz [example-05c](../example-05c/README.md). **Brez gumbov v vmesniku** — uporabnik premika strani z vpisom `/1` ali `/0` na koncu URL-ja v naslovni vrstici brskalnika. example-09 doda klikljive gumbe na isti kodi.

---

## English

### Prerequisites

Just the ESP32 dev board on USB. No `secrets.h` setup — AP credentials are inline (`esp32_1` / `12345678`).

### Synthesis of earlier lessons

Nothing genuinely new in the C++:

- AP mode (`WiFi.softAP`, IP `192.168.4.1`) — from [example-07](../example-07/README.md).
- LED state machine driven by `/1` / `/0` routes — from [example-05c](../example-05c/README.md).
- HTML pages served from LittleFS — from [example-05c](../example-05c/README.md).

The HTML on `/` is intentionally a **single instruction line**, no clickable anchors:

> Vpišite /1 za vklop LED diode ali /0 za izklop, t. j. /1 ali /0 na koncu URL-ja.

This forces the student to manually edit the URL in the browser — useful while learning how HTTP routes work. example-09 swaps this for proper buttons.

### Build, upload, monitor

```bash
pio run --target upload       # firmware
pio run --target uploadfs     # data/ → LittleFS
pio device monitor            # 115200 baud
```

### Trying it from a phone or laptop

1. After upload, the serial monitor prints:
   ```
   LittleFS priklopljen.
   Vzpostavitev esp32 AP - Access Point
   IP naslov esp32 dostopne točke: 192.168.4.1
   ```
2. Join WiFi network **`esp32_1`** with password **`12345678`**. Say "stay connected" to the no-internet warning.
3. Open `http://192.168.4.1/` — you see the instruction text.
4. In the address bar, type `http://192.168.4.1/1` and press Enter. The LED turns on; the page shows *"Prejet ukaz za vklop LED diode."*
5. Type `http://192.168.4.1/0` to turn it off.

---

## Slovensko

### Predpogoji

Samo razvojna ploščica ESP32 na USB-ju. Brez nastavitve `secrets.h` — AP podatki so vrstično (`esp32_1` / `12345678`).

### Sinteza prejšnjih lekcij

V C++ ni nič novega:

- AP način (`WiFi.softAP`, IP `192.168.4.1`) — iz [example-07](../example-07/README.md).
- Stroj stanj LED, krmiljen s potmi `/1` / `/0` — iz [example-05c](../example-05c/README.md).
- HTML strani iz LittleFS — iz [example-05c](../example-05c/README.md).

HTML na `/` je namenoma **ena sama vrstica z navodilom**, brez klikljivih povezav:

> Vpišite /1 za vklop LED diode ali /0 za izklop, t. j. /1 ali /0 na koncu URL-ja.

To študenta prisili, da ročno spremeni URL v brskalniku — koristno pri učenju, kako delujejo HTTP poti. example-09 to zamenja s pravimi gumbi.

### Prevajanje, nalaganje, spremljanje

```bash
pio run --target upload       # firmware
pio run --target uploadfs     # data/ → LittleFS
pio device monitor            # 115200 baud
```

### Preizkus s telefonom ali prenosnikom

1. Po nalaganju serijski monitor izpiše:
   ```
   LittleFS priklopljen.
   Vzpostavitev esp32 AP - Access Point
   IP naslov esp32 dostopne točke: 192.168.4.1
   ```
2. Poveži se na WiFi **`esp32_1`** z geslom **`12345678`**. Pri opozorilu "brez interneta" potrdi.
3. Odpri `http://192.168.4.1/` — vidiš navodilo.
4. V naslovni vrstici vpiši `http://192.168.4.1/1` in pritisni Enter. LED se prižge; stran prikaže *"Prejet ukaz za vklop LED diode."*
5. Vpiši `http://192.168.4.1/0`, da jo ugasneš.
