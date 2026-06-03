# Joined example 02 — Same topology as joined-example-01, but JSON end-to-end

Picks up where [joined-example-01](../joined-example-01/) left off. Same three-piece topology (browser ↔ Node hub ↔ ESP32), but now **the protocol is structured JSON** instead of raw bytes:

- **Node** parses incoming browser messages with `JSON.parse`, validates them, and **routes** by `tipSporočila` to a per-type handler. Bad JSON is dropped at the hub — it never reaches the chip.
- **ESP32** parses each incoming message with **ArduinoJson** and reads `tipSporočila`, `pin`, and `vrednost` as proper fields.

So the LED actually toggles end-to-end with the lecturer's default JSON value, unlike joined-example-01 where it only toggled if you typed plain `1`/`0` into the input field.

Nadgradnja [joined-example-01](../joined-example-01/). Enaka tridelna topologija (brskalnik ↔ Node zvezdišče ↔ ESP32), le da je **protokol zdaj strukturiran JSON** namesto surovih bajtov:

- **Node** dohodna brskalniška sporočila razčleni z `JSON.parse`, jih preveri in jih prek `tipSporočila` **usmeri** v funkcijo za posamezno vrsto. Napačen JSON se zavrne na zvezdišču — do čipa ne pride.
- **ESP32** vsako prejeto sporočilo razčleni z **ArduinoJson** in bere `tipSporočila`, `pin` in `vrednost` kot prava polja.

LED se torej dejansko preklopi od konca do konca s privzeto JSON vrednostjo, za razliko od joined-example-01, kjer se je preklopilo le, če si v vnosno polje vnesla navadno `1`/`0`.

---

## English

### Folder layout

```
joined-example-02/
├── platformio.ini          ← ESP32 build config (outer folder IS the PIO project)
├── src/main.cpp            ← ESP32 sketch — WebSocketsClient + ArduinoJson
├── node-example-09/        ← Node side as a sibling subfolder
│   ├── node-example-09.js    ← Express + two WSS + JSON.parse + per-type router
│   ├── node-example-09.html  ← Browser UI (JSON input + "Pošlji JSON sporočilo")
│   └── package.json
└── README.md
```

### Prerequisites

- ESP32 dev board on USB.
- Laptop on the same WiFi as the chip, with **Node.js (LTS 18+)**.
- `shared/include/secrets.h` configured ([see example-02](../example-02/README.md#one-time-configuration)). The ESP32 uses `WIFI_SSID` / `WIFI_PASSWORD` + `SERVER_HOST` from there.

### What's new vs joined-example-01

| Concept | joined-01 | joined-02 |
| --- | --- | --- |
| Wire format | single char `"1"` / `"0"` | JSON object |
| Node's role | dumb byte relay | parses, validates, routes by type |
| ESP32's parse | `payload[0]` check | `deserializeJson(doc, payload)` |
| Adding a new command | edit chip code | add a `case` + handler in Node, edit chip code |

The pattern change matters more than the format change. Once Node parses the protocol, you can add **server-side logic** (rate limiting, logging, authentication, multi-step state) before forwarding — none of which was possible when Node didn't understand what it was relaying.

### Wire format

Same JSON shape as [homework-03](../homework-03/) and [node-example-07](../node-example-07/):

```json
{ "tipSporočila": "LED", "pin": 2, "vrednost": 1 }
```

`vrednost: 0` turns the LED off. `pin` other than 2 logs a warning on the chip and is ignored (only pin 2 is wired here).

### Running both sides

**1. Start the Node hub first.** From `joined-example-02/node-example-09/`:

```powershell
npm install         # first time only — pulls express, ip, ws
npm start           # node node-example-09.js  (port 80 needs admin PowerShell)
```

You should see three "server up" lines.

**2. Flash and run the ESP32.** From `joined-example-02/` (the outer folder):

```powershell
pio run --target upload
pio device monitor          # 115200 baud
```

The chip joins WiFi and opens a WebSocket to Node:8811. Node logs `ESP32 se je povezal (vrata 8811).`

**3. Open the page** in a browser on the same LAN: `http://<your-laptop-ip>/`. The browser silently opens a WebSocket to Node:8888. Node logs `Brskalnik se je povezal (vrata 8888).`

**4. Click "Pošlji JSON sporočilo".**
- Browser sends the full JSON via ws://node:8888.
- Node parses it, sees `tipSporočila: "LED"`, calls `handleLED(msg)`, which broadcasts the JSON to all ESP32 clients.
- ESP32 receives the JSON, parses it with ArduinoJson, checks `pin == 2`, sets `LED1status = HIGH`.
- The onboard LED on GPIO 2 **turns on**.
- Edit the input to `"vrednost": 0` and click again → LED off.

### Troubleshooting

- **`EACCES: permission denied 0.0.0.0:80`** on Node → port 80 needs admin on Windows. Run PowerShell as admin, or change `HTTP_PORT` to 8080.
- **`(ni veljaven JSON: …)` in Node terminal** → the input field doesn't contain valid JSON. Reset it or fix the syntax.
- **`(nepoznan tipSporočila: …)` in Node terminal** → JSON parsed fine but Node has no handler for that message type. Add a `case` in the switch.
- **`deserializeJson() — napaka` on chip serial** → message arrived but the chip couldn't parse it. Usually means Node forwarded something other than valid JSON (shouldn't happen — Node parses + re-stringifies, so output is always clean).
- **Pin number mismatch** → chip only acts on `pin == 2`. Change `LED1pin` in `main.cpp` if you've wired a different pin, or send a JSON with the right pin.

---

## Slovensko

### Razporeditev map

```
joined-example-02/
├── platformio.ini          ← ESP32 nastavitve gradnje (zunanja mapa JE PIO projekt)
├── src/main.cpp            ← skica ESP32 — WebSocketsClient + ArduinoJson
├── node-example-09/        ← Node stran kot sosednja podmapa
│   ├── node-example-09.js    ← Express + dva WSS + JSON.parse + usmerjevalnik
│   ├── node-example-09.html  ← brskalniški vmesnik (JSON vnos + "Pošlji JSON sporočilo")
│   └── package.json
└── README.md
```

### Predpogoji

- ESP32 razvojna ploščica na USB.
- Prenosnik na istem WiFi-ju kot čip z nameščenim **Node.js (LTS 18+)**.
- Nastavljen `shared/include/secrets.h` ([glej example-02](../example-02/README.md#enkratna-nastavitev)). ESP32 uporabi `WIFI_SSID` / `WIFI_PASSWORD` + `SERVER_HOST` od tam.

### Kaj je novega v primerjavi z joined-example-01

| Koncept | joined-01 | joined-02 |
| --- | --- | --- |
| Oblika sporočila | en znak `"1"` / `"0"` | JSON objekt |
| Vloga Node-a | nepismeni posrednik bajtov | razčleni, preveri, usmerja po vrsti |
| Razčlenjevanje na ESP32 | pregled `payload[0]` | `deserializeJson(doc, payload)` |
| Dodajanje novega ukaza | popravi kodo čipa | dodaj `case` + funkcijo v Node, popravi kodo čipa |

Sprememba vzorca je pomembnejša od spremembe oblike. Ko Node razume protokol, lahko dodaš **strežniško logiko** (omejevanje, beleženje, avtentikacijo, večstopenjsko stanje) pred posredovanjem — kar prej, ko Node ni razumel, ni bilo možno.

### Oblika sporočila

Enaka JSON oblika kot v [homework-03](../homework-03/) in [node-example-07](../node-example-07/):

```json
{ "tipSporočila": "LED", "pin": 2, "vrednost": 1 }
```

`vrednost: 0` ugasne LED. `pin` razen 2 izpiše opozorilo na čipu in se ignorira (priklopljen je samo pin 2).

### Zagon obeh strani

**1. Najprej zaženi Node zvezdišče.** V mapi `joined-example-02/node-example-09/`:

```powershell
npm install         # samo prvič — povleče express, ip, ws
npm start           # node node-example-09.js  (vrata 80 zahtevajo skrbniški PowerShell)
```

Pojavijo se tri vrstice o zagonu strežnikov.

**2. Naloži in poženi ESP32.** Iz mape `joined-example-02/` (zunanje):

```powershell
pio run --target upload
pio device monitor          # 115200 baud
```

Čip se pridruži WiFi-ju in odpre WebSocket na Node:8811. Node izpiše `ESP32 se je povezal (vrata 8811).`

**3. Odpri stran** v brskalniku na istem LAN-u: `http://<IP-tvojega-prenosnika>/`. Brskalnik tiho odpre WebSocket na Node:8888. Node izpiše `Brskalnik se je povezal (vrata 8888).`

**4. Klikni "Pošlji JSON sporočilo".**
- Brskalnik pošlje celoten JSON prek ws://node:8888.
- Node ga razčleni, vidi `tipSporočila: "LED"`, pokliče `handleLED(msg)`, ki JSON razpošlje vsem ESP32 odjemalcem.
- ESP32 JSON prejme, razčleni z ArduinoJson, preveri `pin == 2`, nastavi `LED1status = HIGH`.
- Vgrajena LED na GPIO 2 se **prižge**.
- Spremeni vnos na `"vrednost": 0` in znova klikni → LED ugasne.

### Odpravljanje težav

- **`EACCES: permission denied 0.0.0.0:80`** na strani Node → vrata 80 na Windowsih potrebujejo skrbniške pravice. Zaženi PowerShell kot skrbnik ali spremeni `HTTP_PORT` na 8080.
- **`(ni veljaven JSON: …)` v Node terminalu** → vnosno polje ne vsebuje veljavnega JSON-a. Resetiraj ali popravi sintakso.
- **`(nepoznan tipSporočila: …)` v Node terminalu** → JSON se je razčlenil, vendar Node nima funkcije za to vrsto sporočila. Dodaj `case` v switch.
- **`deserializeJson() — napaka` na serijskem izpisu čipa** → sporočilo je prispelo, čip ga ni mogel razčleniti. Običajno pomeni, da je Node posredoval nekaj, kar ni veljaven JSON (ne bi se smelo zgoditi — Node razčleni in znova serializira, izhod je vedno čist).
- **Neujemanje številke pina** → čip reagira samo na `pin == 2`. Spremeni `LED1pin` v `main.cpp`, če si priklopila drug pin, ali pošlji JSON s pravo številko.
