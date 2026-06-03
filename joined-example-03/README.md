# Joined example 03 — Bidirectional: ESP32 button events also flow back

Picks up where [joined-example-02](../joined-example-02/) left off. Same three-piece topology and same JSON protocol, but **traffic now goes both ways**:

- **Browser → Node → ESP32**: same as joined-02. Click → `{"tipSporočila":"LED",…}` → LED toggles.
- **ESP32 → Node → Browser**: *new*. A physical button on GPIO 18 is read every loop iteration. When the state changes, the chip pushes `{"tipSporočila":"tipka",…}` back through Node, which broadcasts it to every connected browser. The browser updates a `<span id="izpis1">` with the latest value.

So the chip is no longer a pure subscriber — it's a peer that publishes its own events.

Nadgradnja [joined-example-02](../joined-example-02/). Enaka tridelna topologija in enak JSON protokol, vendar **promet teče v obe smeri**:

- **Brskalnik → Node → ESP32**: enako kot v joined-02. Klik → `{"tipSporočila":"LED",…}` → LED se preklopi.
- **ESP32 → Node → Brskalnik**: *novost*. Fizična tipka na GPIO 18 se bere v vsaki zanki. Ob spremembi stanja čip pošlje `{"tipSporočila":"tipka",…}` nazaj prek Node-a, ta pa ga razpošlje vsem priklopljenim brskalnikom. Brskalnik v `<span id="izpis1">` posodobi zadnjo vrednost.

Čip ni več zgolj naročnik — postal je sovrstnik, ki tudi sam objavlja dogodke.

---

## English

### Hardware

- ESP32 dev board on USB.
- A **push button on GPIO 18**, wired so a press pulls the pin to **HIGH** (button between 3.3 V and the pin, with an external pull-down resistor to GND keeping it LOW when unpressed). The sketch uses `pinMode(BUTTON_PIN, INPUT)` — no internal pull, so the external resistor is required.

  Easier alternative: switch to `INPUT_PULLUP` and wire the button to GND. That flips the polarity (HIGH = unpressed), so also swap the `1`/`0` branches in the `dataString` choice.
- Laptop on the same WiFi with Node.js installed.

### Folder layout

```
joined-example-03/
├── platformio.ini          ← ESP32 build config
├── src/main.cpp            ← ESP32 sketch — receives LED commands AND sends button events
├── node-example-10/        ← Node side as a sibling subfolder
│   ├── node-example-10.js    ← Express + two WSS + bidirectional router
│   ├── node-example-10.html  ← Browser UI (JSON input + receive handler)
│   └── package.json
└── README.md
```

### What's new vs joined-example-02

| | joined-02 | joined-03 |
| --- | --- | --- |
| Browser → ESP32 | ✅ JSON | ✅ JSON (unchanged) |
| ESP32 → Browser | ❌ | ✅ new (button events) |
| Node side | one `wss.on("message")` (browser only) | both WSS get `message` handlers + per-direction routing |
| ESP32 side | passive subscriber | publishes button state changes |
| HTML side | send only | send + `ws.onmessage` receive |
| Shared protocol | one type (`"LED"`) | two types (`"LED"`, `"tipka"`) |

The new C++ trick is **edge detection** + **`webSocket.sendTXT(...)`** — only send when state changes, not every loop iteration. The 50 ms `delay` at the end of `loop()` is crude debouncing.

The new Node trick is folding both directions into one generic router: `handleIncoming(source, raw)` is called from either WSS's `message` event, parses the JSON, switches on `tipSporočila`, and dispatches to `handleLED` or `handleTipka`. Each handler then broadcasts to the *other* network of clients.

### Wire format

Two message types now share the same envelope:

| `tipSporočila` | Direction | Example | Side effect |
| --- | --- | --- | --- |
| `"LED"`   | browser → ESP32 | `{"tipSporočila":"LED","pin":2,"vrednost":1}`    | LED on |
| `"tipka"` | ESP32 → browser | `{"tipSporočila":"tipka","pin":18,"vrednost":1}` | UI shows it |

Adding a third type (e.g. `"senzor"` for an analogue reading) means adding one `case` in Node's router + one handler that broadcasts to the right side + one chip-side or browser-side producer.

### Running both sides

**1. Node hub first.** From `joined-example-03/node-example-10/`:

```powershell
npm install
npm start
```

**2. Browser:** open `http://<laptop-ip>/`. Page loads, opens WS to 8888. Node logs `Brskalnik se je povezal (vrata 8888).`

**3. Flash + monitor the ESP32.** From `joined-example-03/`:

```powershell
pio run --target upload
pio device monitor
```

Chip joins WiFi, opens WS to 8811. Node logs `ESP32 se je povezal (vrata 8811).`

**4. Test the LED path** (browser → ESP32):
Click **Pošlji JSON sporočilo**. Default JSON has `"vrednost":1` → LED on. Edit to `"vrednost":0` → click → LED off.

**5. Test the button path** (ESP32 → browser):
Press the GPIO 18 button. The browser page's `Zadnje sporočilo z ESP32` line updates to:
```
{"tipSporočila":"tipka","pin":18,"vrednost":1}
```
Release → updates to `vrednost":0`. Node terminal logs both events as `ESP32 → Node: …`.

### Troubleshooting

- **Button does nothing** → wiring. With `pinMode(INPUT)` and no external pull resistor, the pin floats and `digitalRead` returns garbage. Either add a pull-down to GND, or change to `pinMode(BUTTON_PIN, INPUT_PULLUP)` + wire to GND + flip the polarity check.
- **Browser doesn't update on button press, Node terminal does log `ESP32 → Node: …`** → browser's `ws.onmessage` isn't firing. DevTools (F12) → Console for JS errors, and Network → ws:// row → Messages.
- **`EACCES: permission denied 0.0.0.0:80`** → port 80 needs admin on Windows. Run PowerShell as admin or change `HTTP_PORT` to 8080.
- **`(ni veljaven JSON)` or `(nepoznan tipSporočila)`** in Node terminal → either the input field has malformed JSON, or you sent a message type Node doesn't have a handler for. Add a `case`.

---

## Slovensko

### Strojna oprema

- ESP32 razvojna ploščica na USB.
- **Tipka na GPIO 18**, vezana tako, da pritisk potegne nožico na **HIGH** (tipka med 3.3 V in nožico, z zunanjim pull-down uporom na GND, ki nožico drži na LOW, ko tipka ni pritisnjena). Skica uporablja `pinMode(BUTTON_PIN, INPUT)` — brez notranjega potegnjenega upora, zato je zunanji obvezen.

  Lažja alternativa: spremeni v `INPUT_PULLUP` in tipko priklopi na GND. To obrne polariteto (HIGH = ni pritisnjena), zato je treba zamenjati tudi veji `1`/`0` v izbiri `dataString`.
- Prenosnik na istem WiFi-ju z nameščenim Node.js-jem.

### Razporeditev map

```
joined-example-03/
├── platformio.ini          ← nastavitve gradnje ESP32
├── src/main.cpp            ← skica ESP32 — sprejema ukaze za LED IN pošilja dogodke tipke
├── node-example-10/        ← Node stran kot sosednja podmapa
│   ├── node-example-10.js    ← Express + dva WSS + dvosmerni usmerjevalnik
│   ├── node-example-10.html  ← brskalniški vmesnik (JSON vnos + sprejemnik)
│   └── package.json
└── README.md
```

### Kaj je novega v primerjavi z joined-example-02

| | joined-02 | joined-03 |
| --- | --- | --- |
| Brskalnik → ESP32 | ✅ JSON | ✅ JSON (nespremenjeno) |
| ESP32 → Brskalnik | ❌ | ✅ novo (dogodki tipke) |
| Node stran | en `wss.on("message")` (samo brskalnik) | obe WSS dobita funkciji `message` + usmerjanje po smeri |
| ESP32 stran | pasivni naročnik | objavlja spremembe stanja tipke |
| HTML stran | samo pošiljanje | pošiljanje + `ws.onmessage` sprejem |
| Skupni protokol | ena vrsta (`"LED"`) | dve vrsti (`"LED"`, `"tipka"`) |

Nov C++ trik je **zaznavanje roba** + **`webSocket.sendTXT(...)`** — pošljemo le ob spremembi stanja, ne ob vsaki ponovitvi zanke. 50 ms `delay` na koncu `loop()` deluje kot groba debouncing zaščita.

Nov Node trik je združitev obeh smeri v en generičen usmerjevalnik: `handleIncoming(source, raw)` se kliče iz obeh `wss.on("message")` dogodkov, razčleni JSON, usmerja po `tipSporočila` in pokliče `handleLED` ali `handleTipka`. Vsaka funkcija nato razpošlje *drugemu* omrežju odjemalcev.

### Oblika sporočila

Dve vrsti sporočil delita isto ovojnico:

| `tipSporočila` | Smer | Primer | Stranski učinek |
| --- | --- | --- | --- |
| `"LED"`   | brskalnik → ESP32 | `{"tipSporočila":"LED","pin":2,"vrednost":1}`    | LED se prižge |
| `"tipka"` | ESP32 → brskalnik | `{"tipSporočila":"tipka","pin":18,"vrednost":1}` | vmesnik to prikaže |

Tretja vrsta (npr. `"senzor"` za analogno meritev) = en `case` v Node usmerjevalniku + ena funkcija, ki razpošlje na pravo stran + en proizvajalec sporočila bodisi na čipu bodisi v brskalniku.

### Zagon obeh strani

**1. Najprej Node zvezdišče.** V mapi `joined-example-03/node-example-10/`:

```powershell
npm install
npm start
```

**2. Brskalnik:** odpri `http://<IP-prenosnika>/`. Stran se naloži, odpre WS na 8888. Node izpiše `Brskalnik se je povezal (vrata 8888).`

**3. Naloži in spremljaj ESP32.** Iz mape `joined-example-03/`:

```powershell
pio run --target upload
pio device monitor
```

Čip se pridruži WiFi-ju, odpre WS na 8811. Node izpiše `ESP32 se je povezal (vrata 8811).`

**4. Preizkus LED smeri** (brskalnik → ESP32):
Klikni **Pošlji JSON sporočilo**. Privzeti JSON ima `"vrednost":1` → LED se prižge. Spremeni v `"vrednost":0` → klikni → LED se ugasne.

**5. Preizkus smeri tipke** (ESP32 → brskalnik):
Pritisni tipko na GPIO 18. Vrstica `Zadnje sporočilo z ESP32` se posodobi v:
```
{"tipSporočila":"tipka","pin":18,"vrednost":1}
```
Spusti → posodobi v `vrednost":0`. Node terminal beleži oba dogodka kot `ESP32 → Node: …`.

### Odpravljanje težav

- **Tipka ne dela** → ožičenje. Z `pinMode(INPUT)` in brez zunanjega pull-down upora pin lebdi in `digitalRead` vrne smeti. Dodaj pull-down na GND ali spremeni v `pinMode(BUTTON_PIN, INPUT_PULLUP)` + tipko vežeš na GND + obrneš polariteto v `if/else`.
- **Brskalnik se ne posodobi ob pritisku tipke, Node pa beleži `ESP32 → Node: …`** → `ws.onmessage` v brskalniku ne sproži. DevTools (F12) → Konzola za JS napake, Network → vrstica ws:// → Messages.
- **`EACCES: permission denied 0.0.0.0:80`** → vrata 80 na Windowsih potrebujejo skrbniške pravice. Zaženi PowerShell kot skrbnik ali spremeni `HTTP_PORT` na 8080.
- **`(ni veljaven JSON)` ali `(nepoznan tipSporočila)`** v Node terminalu → bodisi vnosno polje ima napačen JSON, bodisi si poslala vrsto sporočila, za katero Node nima funkcije. Dodaj `case`.
