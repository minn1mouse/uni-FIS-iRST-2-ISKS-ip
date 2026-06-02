# Example 09 — Same as example-08, but with clickable buttons

Sibling of [example-08](../example-08/README.md). Same C++ (AP mode + LED state machine), same `/1` / `/0` routes — only the **HTML changes**: instead of a plain instruction line, the page now has two styled blue button-links the user can click. AP SSID is **`esp32`** (no `_1` suffix) so the two examples can run side by side without colliding.

Sorodno [example-08](../example-08/README.md). Enak C++ (AP način + stroj stanj LED), iste poti `/1` / `/0` — spremenjen je samo **HTML**: namesto navadne navodilne vrstice ima stran zdaj dva slogovno oblikovana modra gumba, ki ju uporabnik klikne. SSID AP je **`esp32`** (brez pripone `_1`), tako da lahko oba primera tečeta hkrati brez kolizije.

---

## English

### Prerequisites

Just the ESP32 dev board on USB. AP credentials are inline (`esp32` / `12345678`).

### What's different from example-08

- **Same `src/main.cpp`** structurally — `WiFi.softAP()`, LittleFS mount, three routes, LED state in `loop()`. Only the AP SSID differs (`esp32` here vs `esp32_1` in example-08).
- **Three styled HTML files in `data/`** — each has the same CSS-styled centred layout with two big blue button-links (`Prižgi LED` / `Ugasni LED`). Clicking a button is identical to typing `/1` or `/0` in the address bar (the buttons are just `<a href="/1">` and `<a href="/0">`).
- The `on.html` and `off.html` pages also carry the buttons, so the user can keep toggling without going back to `/`.

### Why have both 08 and 09?

example-08 teaches the *mechanics* of HTTP routing (the user must understand and type the URL paths). example-09 wraps the same mechanics in a real UI — same backend, friendlier frontend. Running them side by side highlights that **the C++ doesn't change** when you add a UI; only the HTML does.

### Build, upload, monitor

```bash
pio run --target upload       # firmware
pio run --target uploadfs     # data/ → LittleFS
pio device monitor            # 115200 baud
```

### Trying it from a phone or laptop

1. Serial monitor:
   ```
   LittleFS priklopljen.
   Vzpostavitev esp32 AP - Access Point
   IP naslov esp32 dostopne točke: 192.168.4.1
   ```
2. Join WiFi **`esp32`** with password **`12345678`** (note: no `_1`).
3. Open `http://192.168.4.1/`. You see the styled page with two blue buttons.
4. **Click** **Prižgi LED** → LED turns on, page shows confirmation; buttons remain so you can keep toggling.

---

## Slovensko

### Predpogoji

Samo razvojna ploščica ESP32 na USB-ju. AP podatki so vrstično (`esp32` / `12345678`).

### Kaj je drugače kot v example-08

- **Enak `src/main.cpp`** po zgradbi — `WiFi.softAP()`, priklop LittleFS, tri poti, stanje LED v `loop()`. Razlikuje se le SSID AP (`esp32` tukaj proti `esp32_1` v example-08).
- **Tri slogovno oblikovane HTML datoteke v `data/`** — vsaka ima enako sredinsko CSS postavitev z dvema velikima modrima povezavama-gumboma (`Prižgi LED` / `Ugasni LED`). Klik na gumb je enak vpisu `/1` ali `/0` v naslovno vrstico (gumba sta `<a href="/1">` in `<a href="/0">`).
- Tudi strani `on.html` in `off.html` nosita gumba, da lahko uporabnik preklaplja brez vračanja na `/`.

### Zakaj imeti oba — 08 in 09

example-08 uči *mehaniko* HTTP usmerjanja (študent mora razumeti in vpisati URL poti). example-09 isto mehaniko zavije v pravi vmesnik — isti backend, prijaznejši frontend. Hkratno tečenje obeh primerov pokaže, da se **C++ ne spremeni**, ko dodaš vmesnik; spremeni se le HTML.

### Prevajanje, nalaganje, spremljanje

```bash
pio run --target upload       # firmware
pio run --target uploadfs     # data/ → LittleFS
pio device monitor            # 115200 baud
```

### Preizkus s telefonom ali prenosnikom

1. Serijski monitor:
   ```
   LittleFS priklopljen.
   Vzpostavitev esp32 AP - Access Point
   IP naslov esp32 dostopne točke: 192.168.4.1
   ```
2. Poveži se na WiFi **`esp32`** z geslom **`12345678`** (pozor: brez `_1`).
3. Odpri `http://192.168.4.1/`. Vidiš slogovno oblikovano stran z dvema modrima gumboma.
4. **Klikni** **Prižgi LED** → LED se prižge, stran prikaže potrditev; gumba ostaneta, tako da lahko nadaljuješ s preklapljanjem.
