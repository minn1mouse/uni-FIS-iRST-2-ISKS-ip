# Joined example 01 — Browser → Node → ESP32 over WebSockets

The first **joined** example: ESP32 sketch (`src/main.cpp`) AND Node server (`node-example-08/`) in one folder, designed to run together. Closes the loop that the standalone Node WebSocket examples and `homework-03` only showed half of.

**Topology:**

```
   Browser <---ws:8888---> [ Node hub ] <---ws:8811---> ESP32
                                ^
                                |
                          HTTP :80 serves the page
```

The browser opens the page (Express, port 80), then opens a WebSocket back to Node on **port 8888**. The ESP32 opens its own WebSocket to Node on **port 8811**. The Node process **forwards** every message from the browser side, unchanged, to every connected ESP32. The ESP32 looks at the first byte of the message (`'1'` or `'0'`) and toggles its LED.

Prvi **združen** primer: ESP32 skica (`src/main.cpp`) IN Node strežnik (`node-example-08/`) v isti mapi, namenjena skupnemu izvajanju. Sklene krog, ki so ga samostojni Node WebSocket primeri in `homework-03` pokazali le napol.

**Topologija:**

```
   Brskalnik <---ws:8888---> [ Node hub ] <---ws:8811---> ESP32
                                  ^
                                  |
                          HTTP :80 streže stran
```

Brskalnik odpre stran (Express, vrata 80), nato pa odpre WebSocket nazaj na Node na **vrata 8888**. ESP32 sam odpre svoj WebSocket na Node na **vrata 8811**. Node proces nespremenjeno **posreduje** vsako sporočilo s strani brskalnika vsem priklopljenim ESP32. ESP32 pogleda prvi bajt sporočila (`'1'` ali `'0'`) in preklopi LED.

---

## English

### Folder layout

```
joined-example-01/
├── platformio.ini          ← ESP32 build config (outer folder IS the PIO project)
├── src/main.cpp            ← ESP32 sketch — WebSocketsClient → 8811, payload[0] → LED
├── node-example-08/        ← Node side as a sibling subfolder
│   ├── node-example-08.js    ← Express + two WebSocketServers + forwarding
│   ├── node-example-08.html  ← Browser UI (input field with default JSON + "Pošlji sporočilo")
│   └── package.json
└── README.md               ← this file
```

The outer folder is the PlatformIO project. `cd` into it for `pio run` / `pio device monitor`. The Node piece is in `node-example-08/`; `cd` in there for `npm install` / `npm start`.

### Prerequisites

- ESP32 dev board on USB.
- A laptop on the same WiFi as the chip, with **Node.js (LTS 18+)** installed.
- `shared/include/secrets.h` configured ([see example-02](../example-02/README.md#one-time-configuration)). The ESP32 uses `WIFI_SSID` / `WIFI_PASSWORD` + `SERVER_HOST` from there.
- `SERVER_PORT 8811` is set inline in `src/main.cpp` — matches the Node side's `WS_ESP32` constant.

### What's new vs the standalone examples

| Concept | First appeared in | Used here as |
| --- | --- | --- |
| ESP32 WebSocketsClient + first-byte command | [example-11](../example-11/README.md) | the chip subscriber |
| Express + ws on parallel ports | [node-example-07](../node-example-07/README.md) | the HTTP + browser-WS server |
| Two parallel WebSocketServers | (this example) | one server per client type |
| **Cross-socket forwarding** | (this example) | the hub's whole job |

The **new** idea here is the forwarding step inside the Node `wssBrowser` `message` handler: `wssEsp32.clients.forEach((esp) => esp.send(text))`. Without that one loop, the two parallel WebSocket servers are unrelated.

### Wire format

The HTML input field is pre-filled with a JSON-shaped value carried over from the lecturer's example:

```json
{"tipSporočila": "LED", "pin": 2, "vrednost": 1}
```

…but the ESP32 only looks at `payload[0]`. Since `{` is neither `'1'` nor `'0'`, **the LED doesn't toggle from the default value** — clicking the button proves the pipe is alive (you'll see the message logged on the chip's serial monitor) without actually driving anything. This matches the lecturer's example-08 intent: topology demo, no actuator side-effect.

**To actually see the LED toggle**, edit the input field value to just `1` (no quotes, no braces) and click — chip receives `payload[0] == '1'`, LED turns on. Then change to `0` and click again — off. The full JSON form is shown end-to-end in `node-example-07` (server-side parse) and `homework-03` (chip-side parse with ArduinoJson) when you're ready for that lesson.

### Running both sides

**1. Start the Node hub first.** From `joined-example-01/node-example-08/`:

```powershell
npm install         # first time only — pulls express, ip, ws
npm start           # node node-example-08.js  (port 80 needs admin PowerShell)
```

You should see three lines confirming all three servers are up.

**2. Open the page** in a browser on the same LAN: `http://<your-laptop-ip>/`. The browser silently opens a WebSocket to Node:8888; the Node terminal logs `Brskalnik se je povezal (vrata 8888).`

**3. Flash and run the ESP32.** From `joined-example-01/` (the outer folder):

```powershell
pio run --target upload
pio device monitor          # 115200 baud
```

Once the chip joins WiFi it'll open a WebSocket to Node:8811; the Node terminal logs `ESP32 se je povezal (vrata 8811).`

**4. Click "Pošlji sporočilo" in the browser.**
- Browser sends the current input-field value (default JSON) via ws://node:8888.
- Node forwards it to ws://node:8811.
- ESP32 receives it and logs `Prejeto: {…}` on its serial monitor.
- LED **doesn't toggle from the default value** (`{` ≠ `'1'`/`'0'`). To see the LED react, edit the input value to `1` or `0` and click again — see the *Wire format* section above.

### Troubleshooting

- **`EACCES: permission denied 0.0.0.0:80`** on the Node side → port 80 needs admin on Windows. Either run PowerShell as administrator, or change `HTTP_PORT` to 8080 in `node-example-08.js` and visit `http://<ip>:8080/`.
- **`Connection reset by peer` / `Povezava s strežnikom prekinjena` on the ESP32 side** → Node isn't running yet (start it first), or `SERVER_HOST` in `secrets.h` doesn't match the Node machine's actual LAN IP. Check with `ipconfig`.
- **Button works, Node logs the forward, but ESP32 doesn't react** → check the Node `wssEsp32.clients` is non-empty (look for the `ESP32 se je povezal` line). If the chip never connected, the `forEach` loop has nothing to send to. Restart the chip.
- **Buttons click but the chip serial doesn't show `Prejeto`** → the Node side isn't forwarding (look for `Brskalnik → ESP32` in its terminal). Verify both ws connections logged.

---

## Slovensko

### Razporeditev map

```
joined-example-01/
├── platformio.ini          ← nastavitve gradnje ESP32 (zunanja mapa JE PIO projekt)
├── src/main.cpp            ← skica ESP32 — WebSocketsClient → 8811, payload[0] → LED
├── node-example-08/        ← Node stran kot sosednja podmapa
│   ├── node-example-08.js    ← Express + dva WebSocketServerja + posredovanje
│   ├── node-example-08.html  ← brskalniški vmesnik (vnosno polje z privzetim JSON + "Pošlji sporočilo")
│   └── package.json
└── README.md               ← ta datoteka
```

Zunanja mapa je PlatformIO projekt. V njej teče `pio run` / `pio device monitor`. Node del je v `node-example-08/`; tam teče `npm install` / `npm start`.

### Predpogoji

- ESP32 razvojna ploščica na USB.
- Prenosnik na istem WiFi-ju kot čip z nameščenim **Node.js (LTS 18+)**.
- Nastavljen `shared/include/secrets.h` ([glej example-02](../example-02/README.md#enkratna-nastavitev)). ESP32 uporabi `WIFI_SSID` / `WIFI_PASSWORD` + `SERVER_HOST` od tam.
- `SERVER_PORT 8811` je nastavljen vrstično v `src/main.cpp` — ujema se z Node konstanto `WS_ESP32`.

### Kaj je novega v primerjavi s samostojnimi primeri

| Koncept | Prvič v | Tukaj kot |
| --- | --- | --- |
| ESP32 WebSocketsClient + ukaz po prvem znaku | [example-11](../example-11/README.md) | naročnik na čipu |
| Express + ws na vzporednih vratih | [node-example-07](../node-example-07/README.md) | HTTP in brskalniški WS strežnik |
| Dva vzporedna WebSocketServerja | (ta primer) | po en strežnik za vrsto odjemalca |
| **Posredovanje med vtičnikoma** | (ta primer) | naloga zvezdišča |

**Novost** tega primera je posredovanje znotraj Node-jeve funkcije za sporočila brskalnika: `wssEsp32.clients.forEach((esp) => esp.send(text))`. Brez tega ene zanke sta dva vzporedna WebSocket strežnika nepovezana.

### Oblika sporočila

Vnosno polje HTML je vnaprej napolnjeno z JSON vrednostjo iz predavateljevega primera:

```json
{"tipSporočila": "LED", "pin": 2, "vrednost": 1}
```

…ESP32 pa pogleda le `payload[0]`. Ker `{` ni niti `'1'` niti `'0'`, **LED se s privzeto vrednostjo ne preklopi** — klik na gumb dokaže, da povezava deluje (na serijskem monitorju čipa vidiš sporočilo), brez dejanske akcije na izhodu. To se ujema z namenom predavateljevega example-08: demo topologije brez stranskega učinka na aktuator.

**Da LED dejansko preklopi**, v vnosnem polju zamenjaj vrednost z `1` (brez narekovajev, brez oklepajev) in klikni — čip prejme `payload[0] == '1'`, LED se prižge. Nato spremeni v `0` in znova klikni — ugasne. Polno JSON obliko od konca do konca pokažeta `node-example-07` (razčlenjevanje na strežniku) in `homework-03` (razčlenjevanje na čipu z ArduinoJson), ko si pripravljena na to lekcijo.

### Zagon obeh strani

**1. Najprej zaženi Node zvezdišče.** V mapi `joined-example-01/node-example-08/`:

```powershell
npm install         # samo prvič — povleče express, ip, ws
npm start           # node node-example-08.js  (vrata 80 zahtevajo skrbniški PowerShell)
```

Pojavijo se tri vrstice, ki potrjujejo, da so vsi trije strežniki tečejo.

**2. Odpri stran** v brskalniku na istem LAN-u: `http://<IP-tvojega-prenosnika>/`. Brskalnik tiho odpre WebSocket na Node:8888; Node terminal izpiše `Brskalnik se je povezal (vrata 8888).`

**3. Naloži in poženi ESP32.** Iz mape `joined-example-01/` (zunanje):

```powershell
pio run --target upload
pio device monitor          # 115200 baud
```

Ko se čip pridruži WiFi-ju, odpre WebSocket na Node:8811; Node terminal izpiše `ESP32 se je povezal (vrata 8811).`

**4. Klikni "Pošlji sporočilo" v brskalniku.**
- Brskalnik pošlje trenutno vrednost iz vnosnega polja (privzeto JSON) prek ws://node:8888.
- Node ga posreduje na ws://node:8811.
- ESP32 ga prejme in v serijski monitor zabeleži `Prejeto: {…}`.
- LED se **s privzeto vrednostjo ne preklopi** (`{` ≠ `'1'`/`'0'`). Da LED reagira, spremeni vrednost na `1` ali `0` in znova klikni — glej zgornji razdelek *Oblika sporočila*.

### Odpravljanje težav

- **`EACCES: permission denied 0.0.0.0:80`** na strani Node → vrata 80 na Windowsih potrebujejo skrbniške pravice. Zaženi PowerShell kot skrbnik ali pa v `node-example-08.js` spremeni `HTTP_PORT` na 8080 in obišči `http://<ip>:8080/`.
- **`Connection reset by peer` / `Povezava s strežnikom prekinjena` na strani ESP32** → Node še ne teče (najprej ga zaženi) ali `SERVER_HOST` v `secrets.h` se ne ujema z dejanskim IP-jem Node računalnika. Preveri z `ipconfig`.
- **Gumb dela, Node beleži posredovanje, ESP32 pa se ne odzove** → preveri, da `wssEsp32.clients` ni prazen (poišči vrstico `ESP32 se je povezal`). Če se čip ni povezal, zanka `forEach` nima komu pošiljati. Znova zaženi čip.
- **Gumbi delajo, a v serijskem izpisu čipa ni `Prejeto`** → Node ne posreduje (poišči vrstico `Brskalnik → ESP32` v njegovem terminalu). Preveri, da sta obe ws povezavi zabeleženi.
