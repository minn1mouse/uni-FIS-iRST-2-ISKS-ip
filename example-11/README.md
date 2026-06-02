# Example 11 — WebSocket with a real client UI

Picks up where [example-10](../example-10/README.md) left off. The C++ side is essentially unchanged — same `WebSocketsServer` on port 81, same `onWebSocketEvent` handling `WStype_CONNECTED` / `WStype_DISCONNECTED` / `WStype_TEXT`. The new piece is the **browser client**: `data/index.html` now contains two buttons and a small JavaScript block that opens a WebSocket on page load and sends `"1"` / `"0"` when the buttons are clicked. No more dev-tools or `wscat` to test — it's a real one-page web app.

Nadaljevanje [example-10](../example-10/README.md). C++ stran je tako rekoč nespremenjena — isti `WebSocketsServer` na vratih 81, isti `onWebSocketEvent`, ki obravnava `WStype_CONNECTED` / `WStype_DISCONNECTED` / `WStype_TEXT`. Novost je **odjemalec v brskalniku**: `data/index.html` ima zdaj dva gumba in kratek JavaScript blok, ki ob nalaganju strani odpre WebSocket in pošlje `"1"` / `"0"`, ko klikneš gumb. Nič več razvojnih orodij ali `wscat`-a za preizkus — to je prava enostranska spletna aplikacija.

---

## English

### Prerequisites

Same hardware and `secrets.h` setup as the earlier networked examples ([example-02](../example-02/README.md#one-time-configuration)). The PIO library `links2004/WebSockets@^2.7.3` is already declared in `platformio.ini` (carried over from example-10).

### What's new in this example

The lesson lives entirely in `data/index.html`, not the C++. Three new things in the page:

1. **Two `<button>` elements** with `onclick` handlers that fire local JS functions:
   ```html
   <button onclick="prižgi()">Prižgi LED</button>
   <button onclick="ugasni()">Ugasni LED</button>
   ```
2. **A WebSocket opened in `<script>`** when the page loads:
   ```js
   let povezava = new WebSocket('ws://' + location.hostname + ':81/');
   ```
   `location.hostname` resolves at runtime to the IP you typed into the address bar (the ESP32's IP), so the same HTML works on any network without hand-editing.
3. **Two functions that send a single character** over the socket:
   ```js
   function prižgi() { povezava.send("1"); }
   function ugasni() { povezava.send("0"); }
   ```
   The C++ `WStype_TEXT` handler reads `payload[0]` and toggles `LED1status`.

The C++ in `src/main.cpp` is identical in shape to example-10 — nothing new on the server side.

### Build, upload, monitor

```bash
pio run --target upload       # firmware
pio run --target uploadfs     # data/ → LittleFS
pio device monitor            # 115200 baud
```

### Trying it from a browser

1. After upload, the monitor prints:
   ```
   LittleFS priklopljen.
   Povezovanje na home omrežje: TP-Link_1C01
   ...
   Povezava z WiFi omrežjem je vzpostavljena.
   Moj IP naslov je: 192.168.x.x
   HTTP strežnik je zagnan na vratih 80.
   WebSocket strežnik je zagnan na vratih 81.
   ```
2. Open `http://192.168.x.x/`. You see the heading and two plain buttons. **The WebSocket opens silently** — check the monitor for the `[0] Klient povezan z IP: ...` line.
3. Click **Prižgi LED** → monitor logs `[0] Prejeto sporočilo: 1`, LED turns on. Click **Ugasni LED** → message `0`, LED off. No page reload, no flicker.

### Troubleshooting

- **Buttons do nothing, no `Klient povezan` in the serial log** → the WebSocket didn't open. F12 → Console in the browser will show why (CORS, ws:// vs https:// mixed content, wrong port, etc).
- **`prižgi()` / `ugasni()` JS errors** → the Slovene function names (`ž` is a non-ASCII identifier) are valid in modern JS but can confuse older tooling. If anything complains, rename them to `turnOn` / `turnOff` and update the `onclick` attributes to match.
- **`LittleFS mount failed`** → run `pio run --target uploadfs`.

---

## Slovensko

### Predpogoji

Enaka strojna oprema in nastavitev `secrets.h` kot v prejšnjih omrežnih primerih ([example-02](../example-02/README.md#enkratna-nastavitev)). PIO knjižnica `links2004/WebSockets@^2.7.3` je že določena v `platformio.ini` (prenesena iz example-10).

### Kaj je novega v tem primeru

Lekcija je v celoti v `data/index.html`, ne v C++. Tri nove stvari na strani:

1. **Dva `<button>` elementa** s `onclick` funkcijami, ki kličeta lokalne JS funkcije:
   ```html
   <button onclick="prižgi()">Prižgi LED</button>
   <button onclick="ugasni()">Ugasni LED</button>
   ```
2. **WebSocket, odprt v `<script>`** ob nalaganju strani:
   ```js
   let povezava = new WebSocket('ws://' + location.hostname + ':81/');
   ```
   `location.hostname` se ob izvajanju zamenja z IP-jem, ki si ga vpisal v naslovno vrstico (IP ESP32-ja), tako da ista HTML deluje na katerikoli mreži brez ročnih popravkov.
3. **Dve funkciji, ki pošljeta po en znak** prek vtičnika:
   ```js
   function prižgi() { povezava.send("1"); }
   function ugasni() { povezava.send("0"); }
   ```
   C++ funkcija za `WStype_TEXT` prebere `payload[0]` in preklopi `LED1status`.

C++ v `src/main.cpp` je po obliki enak example-10 — na strežniški strani ni nič novega.

### Prevajanje, nalaganje, spremljanje

```bash
pio run --target upload       # firmware
pio run --target uploadfs     # data/ → LittleFS
pio device monitor            # 115200 baud
```

### Preizkus iz brskalnika

1. Po nalaganju monitor izpiše:
   ```
   LittleFS priklopljen.
   Povezovanje na home omrežje: TP-Link_1C01
   ...
   Povezava z WiFi omrežjem je vzpostavljena.
   Moj IP naslov je: 192.168.x.x
   HTTP strežnik je zagnan na vratih 80.
   WebSocket strežnik je zagnan na vratih 81.
   ```
2. Odpri `http://192.168.x.x/`. Vidiš naslov in dva navadna gumba. **WebSocket se tiho odpre** — v monitorju preveri vrstico `[0] Klient povezan z IP: ...`.
3. Klikni **Prižgi LED** → monitor zabeleži `[0] Prejeto sporočilo: 1`, LED se prižge. Klikni **Ugasni LED** → sporočilo `0`, LED ugasne. Brez osveževanja strani, brez utripa.

### Odpravljanje težav

- **Gumbi ne delujejo, v serijskem izpisu ni vrstice `Klient povezan`** → WebSocket se ni odprl. F12 → Konzola v brskalniku bo pokazala razlog (CORS, ws:// vs https:// mešana vsebina, napačna vrata ipd.).
- **Napake JS pri `prižgi()` / `ugasni()`** → slovenska imena funkcij (`ž` je ne-ASCII identifikator) so veljavna v sodobnem JS, vendar lahko zmedejo starejša orodja. Če se kaj pritožuje, jih preimenuj v `turnOn` / `turnOff` in popravi `onclick` atribute.
- **`LittleFS mount failed`** → zaženi `pio run --target uploadfs`.
