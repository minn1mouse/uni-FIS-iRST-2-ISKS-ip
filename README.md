# uni-FIS-iRST-2-ISKS-ip

Coursework for **FIS Novo mesto — VS RST 2 / ISK: Internet stvari (IoT) in Kiberfizični sistemi (CPS)**. A collection of small, self-contained examples that progress from blinking an LED to bidirectional Browser ↔ Node ↔ ESP32 communication over WebSockets.

Učno gradivo za **FIS Novo mesto — VS RST 2 / ISK: Internet stvari (IoT) in Kiberfizični sistemi (CPS)**. Zbirka majhnih, samostojnih primerov, ki napredujejo od utripajoče LED do dvosmerne komunikacije Brskalnik ↔ Node ↔ ESP32 prek WebSocketov.

---

## English

### Repository structure

Four parallel series, each in its own top-level folders:

- **`example-NN/`** — ESP32 sketches (PlatformIO, Arduino framework). Chip-side only.
- **`node-example-NN/`** — Node.js projects. Laptop-side only. Each has its own `package.json`.
- **`joined-example-NN/`** — full pairs: outer folder is a PlatformIO project for the chip, with a `node-example-MM/` subfolder for the matching Node hub. Designed to run together.
- **`homework-NN/`** — graded homework assignments.

Letter suffixes (e.g. `05a`, `03a`, `04a`) mean **"variant that reuses the previous example's chip code but changes the front-end"** — so you can diff just the HTML/CSS/JS without re-reading the same C++ twice.

### Shared infrastructure

`shared/include/` and `shared/node/` hold credentials and the developer's LAN IP — both **gitignored**, with `.example` templates committed.

| File | Used by | Purpose |
|---|---|---|
| [shared/include/secrets.h](shared/include/secrets.example.h) | ESP32 examples | `WIFI_SSID`, `WIFI_PASSWORD`, `WIFI_NETWORK_LABEL`, `SERVER_HOST`. Toggle home vs hotspot with one `#define`. |
| [shared/include/wifi_log.h](shared/include/wifi_log.h) | ESP32 examples | `printWifiTarget()` — logs which network the chip is about to join. |
| [shared/node/config.js](shared/node/config.example.js) | Node examples | Exports `SERVER_HOST` only; port is per-example inline. |

See [CLAUDE.md](CLAUDE.md) for the full shared-infrastructure rules (`build_flags = -I"${PROJECT_DIR}/../shared/include"` — **path must be quoted** because the repo lives under a path containing a space).

### `example-NN/` — ESP32 chip-side

| # | Topic |
|---|---|
| [example-01](example-01/) | Blink — onboard LED on GPIO 2. |
| [example-02](example-02/) | Connect to WiFi (STA mode), log IP + MAC. |
| [example-03](example-03/) | HTTP server on the chip; root returns plain text. |
| [example-04](example-04/) | HTTP server returning HTML with UTF-8 (Slovene characters). |
| [example-05a](example-05a/) | Multiple HTTP routes. |
| [example-05b](example-05b/) | Control the onboard LED over HTTP (the "IoT hello world"). |
| [example-05c](example-05c/) | Serve HTML from a flash filesystem (LittleFS). |
| [example-06](example-06/) | Styled HTML pages from LittleFS (CSS + multiple files). |
| [example-07](example-07/) | ESP32 as a WiFi access point (soft AP). |
| [example-08](example-08/) | Control the LED over HTTP in AP mode (URL-only). |
| [example-09](example-09/) | Same as 08, but with clickable buttons in the page. |
| [example-10](example-10/) | WebSocket **server** on the chip (no UI yet — test with a client tool). |
| [example-11](example-11/) | WebSocket server **with** a real client UI. |
| [example-12](example-12/) | Read the potentiometer (ADC on GPIO 34) and stream values to the serial monitor. |

### `node-example-NN/` — Laptop-side

| # | Topic |
|---|---|
| [node-example-00](node-example-00/) | Minimal HTTP "Hello world" server with the built-in `http` module. |
| [node-example-01](node-example-01/) | HTTP server bound to a specific LAN IP (`SERVER_HOST`). |
| [node-example-02](node-example-02/) | Serve a static HTML file from disk. |
| [node-example-03](node-example-03/) | Multiple routes, each serving its own HTML file. |
| [node-example-04](node-example-04/) | Same routing idea, but with Express. |
| [node-example-05](node-example-05/) | Express HTTP server + a parallel WebSocket server. |
| [node-example-06](node-example-06/) | WebSocket with a real client UI (browser → server). |
| [node-example-07](node-example-07/) | Parse incoming WebSocket messages as JSON. |

### `joined-example-NN/` — Chip + Node together

Two `WebSocketServer` instances live in the Node hub: **8888** for browsers, **8811** for the ESP32. A `handleIncoming(source, raw)` router parses each message's `tipSporočila` and forwards it to the right side.

| # | Topic |
|---|---|
| [joined-example-01](joined-example-01/) | First end-to-end pipeline: Browser → Node → ESP32. |
| [joined-example-02](joined-example-02/) | Same topology, but JSON end-to-end. |
| [joined-example-03](joined-example-03/) | Bidirectional — chip's button (GPIO 18) pushes events back to the browser. |
| [joined-example-03a](joined-example-03a/) | Same chip code as 03; browser HTML adds a canvas visualisation of the button state. |
| [joined-example-04](joined-example-04/) | Live potentiometer stream from the chip to the browser at ~20 Hz. |
| [joined-example-04a](joined-example-04a/) | Same chip code as 04; browser HTML plots every reading as a live line graph. |

### `homework-NN/`

| # | Topic |
|---|---|
| [homework-01](homework-01/) | Three-LED HTTP control with templated HTML. |
| [homework-02](homework-02/) | Three-LED HTTP control, single-message UI. |
| [homework-03](homework-03/) | Meta-pointer — the deliverable consists of [joined-example-02](joined-example-02/), [03](joined-example-03/) and [03a](joined-example-03a/). |
| [homework-04](homework-04/) | Closed-loop LED control with photoresistor feedback. |

### Building and running

Each subfolder is self-contained.

```powershell
# ESP32 example (run from inside the folder)
pio run                       # build
pio run --target upload       # flash
pio device monitor            # serial monitor (baud per platformio.ini)

# Node example (run from inside the folder)
npm install
npm start
```

See each subproject's `README.md` for full build/run details and any wiring.

---

## Slovensko

### Struktura repozitorija

Štiri vzporedne serije, vsaka v svojih mapah na najvišjem nivoju:

- **`example-NN/`** — skice za ESP32 (PlatformIO, ogrodje Arduino). Samo stran čipa.
- **`node-example-NN/`** — projekti Node.js. Samo stran prenosnika. Vsak ima svoj `package.json`.
- **`joined-example-NN/`** — celotni pari: zunanja mapa je PlatformIO projekt za čip, znotraj je podmapa `node-example-MM/` z ujemajočim Node zvezdiščem. Zasnovano za skupni zagon.
- **`homework-NN/`** — domače naloge.

Črkovne pripone (npr. `05a`, `03a`, `04a`) pomenijo **"različica, ki znova uporabi kodo prejšnjega primera na čipu, spremeni pa stran v brskalniku"** — tako lahko primerjaš samo HTML/CSS/JS, brez ponovnega prebiranja istega C++.

### Skupna infrastruktura

Mapi `shared/include/` in `shared/node/` hranita poverilnice in razvijalčev IP v lokalnem omrežju — obe sta **izven gita** (`.gitignore`), v repozitoriju pa so predloge `.example`.

| Datoteka | Uporabljajo | Namen |
|---|---|---|
| [shared/include/secrets.h](shared/include/secrets.example.h) | ESP32 primeri | `WIFI_SSID`, `WIFI_PASSWORD`, `WIFI_NETWORK_LABEL`, `SERVER_HOST`. Preklop med domačim omrežjem in hotspotom z enim `#define`. |
| [shared/include/wifi_log.h](shared/include/wifi_log.h) | ESP32 primeri | `printWifiTarget()` — izpiše, na katero omrežje se bo čip povezal. |
| [shared/node/config.js](shared/node/config.example.js) | Node primeri | Izvoz samo `SERVER_HOST`; vrata so v vsakem primeru posebej. |

Celotna pravila so v [CLAUDE.md](CLAUDE.md) (`build_flags = -I"${PROJECT_DIR}/../shared/include"` — **pot mora biti v narekovajih**, ker je v poti repozitorija presledek).

### `example-NN/` — stran ESP32

| # | Tema |
|---|---|
| [example-01](example-01/) | Utripanje vgrajene LED na GPIO 2. |
| [example-02](example-02/) | Povezovanje na WiFi (način STA), izpis IP in MAC. |
| [example-03](example-03/) | HTTP strežnik na čipu; koren vrača golo besedilo. |
| [example-04](example-04/) | HTTP strežnik vrača HTML z UTF-8 (slovenski znaki). |
| [example-05a](example-05a/) | Več HTTP poti. |
| [example-05b](example-05b/) | Krmiljenje vgrajene LED prek HTTP ("IoT hello world"). |
| [example-05c](example-05c/) | Postrežba HTML iz datotečnega sistema na flashu (LittleFS). |
| [example-06](example-06/) | Oblikovane HTML strani iz LittleFS (CSS + več datotek). |
| [example-07](example-07/) | ESP32 kot dostopna točka WiFi (soft AP). |
| [example-08](example-08/) | Krmiljenje LED prek HTTP v načinu AP (samo URL). |
| [example-09](example-09/) | Enako kot 08, le da s klikljivimi gumbi v strani. |
| [example-10](example-10/) | WebSocket **strežnik** na čipu (brez UI — preizkusi z orodjem za odjemalca). |
| [example-11](example-11/) | WebSocket strežnik **s** pravim odjemalcem v brskalniku. |
| [example-12](example-12/) | Branje potenciometra (ADC na GPIO 34) in pošiljanje vrednosti v serijski monitor. |

### `node-example-NN/` — stran prenosnika

| # | Tema |
|---|---|
| [node-example-00](node-example-00/) | Minimalni HTTP "Hello world" strežnik z vgrajenim modulom `http`. |
| [node-example-01](node-example-01/) | HTTP strežnik, vezan na določen IP v lokalnem omrežju (`SERVER_HOST`). |
| [node-example-02](node-example-02/) | Postrežba statične HTML datoteke z diska. |
| [node-example-03](node-example-03/) | Več poti, vsaka servira svojo HTML datoteko. |
| [node-example-04](node-example-04/) | Enaka logika usmerjanja, vendar z Express. |
| [node-example-05](node-example-05/) | Express HTTP strežnik + vzporedni WebSocket strežnik. |
| [node-example-06](node-example-06/) | WebSocket s pravim odjemalcem v brskalniku (brskalnik → strežnik). |
| [node-example-07](node-example-07/) | Razčlenjevanje prejetih WebSocket sporočil kot JSON. |

### `joined-example-NN/` — čip in Node skupaj

V Node zvezdišču tečeta dva `WebSocketServer`-ja: **8888** za brskalnike, **8811** za ESP32. Usmerjevalnik `handleIncoming(source, raw)` razčleni `tipSporočila` vsakega sporočila in ga posreduje na pravo stran.

| # | Tema |
|---|---|
| [joined-example-01](joined-example-01/) | Prva celovita pot: Brskalnik → Node → ESP32. |
| [joined-example-02](joined-example-02/) | Enaka topologija, vendar JSON od konca do konca. |
| [joined-example-03](joined-example-03/) | Dvosmerno — tipka na čipu (GPIO 18) pošilja dogodke nazaj v brskalnik. |
| [joined-example-03a](joined-example-03a/) | Enaka koda na čipu kot 03; HTML v brskalniku doda izris stanja tipke na platnu. |
| [joined-example-04](joined-example-04/) | Pretok meritev potenciometra s čipa v brskalnik pri ~20 Hz. |
| [joined-example-04a](joined-example-04a/) | Enaka koda na čipu kot 04; HTML v brskalniku izriše vsako meritev kot živi linijski graf. |

### `homework-NN/`

| # | Tema |
|---|---|
| [homework-01](homework-01/) | Krmiljenje treh LED prek HTTP s predlogo HTML. |
| [homework-02](homework-02/) | Krmiljenje treh LED prek HTTP, vmesnik z enim sporočilom. |
| [homework-03](homework-03/) | Meta-kazalec — oddajo sestavljajo [joined-example-02](joined-example-02/), [03](joined-example-03/) in [03a](joined-example-03a/). |
| [homework-04](homework-04/) | Zaprtozančno krmiljenje LED s povratno informacijo iz fotoupora. |

### Prevajanje in zagon

Vsaka podmapa je samostojna.

```powershell
# ESP32 primer (zaženi iz mape)
pio run                       # prevedi
pio run --target upload       # naloži na čip
pio device monitor            # serijski monitor (baud iz platformio.ini)

# Node primer (zaženi iz mape)
npm install
npm start
```

Podrobnosti o prevajanju, zagonu in žicah so v `README.md` vsakega podprojekta.
