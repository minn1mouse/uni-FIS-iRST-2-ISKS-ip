# Example 10 — WebSocket server (server side only, no UI yet)

Introduces **WebSockets** — a persistent bidirectional connection that runs alongside the HTTP server (HTTP on port 80, WebSocket on port 81). This example focuses on the **server-side plumbing**: setting up `WebSocketsServer`, writing the event callback (`onWebSocketEvent`) that reacts to client connects, disconnects, and text messages, and pumping `webSocket.loop()` from the main loop. The HTML at `/` is intentionally a single heading — there is **no client UI yet**, so you'll need browser dev tools / a separate client to actually exchange messages. [example-11](../example-11/README.md) layers the UX on top of the same server.

Uvaja **WebSocket-e** — trajno dvosmerno povezavo, ki teče vzporedno s HTTP strežnikom (HTTP na vratih 80, WebSocket na vratih 81). Ta primer se osredotoča na **strežniško stran**: postavitev `WebSocketsServer`, pisanje povratne funkcije (`onWebSocketEvent`), ki reagira na priklope, odklope in besedilna sporočila, ter klicanje `webSocket.loop()` iz glavne zanke. HTML na `/` je namenoma samo en naslov — **uporabniškega vmesnika še ni**, zato potrebuješ razvojna orodja v brskalniku ali ločenega odjemalca za izmenjavo sporočil. [example-11](../example-11/README.md) doda vmesnik na isto strežniško osnovo.

---

## English

### Prerequisites

Same hardware and `secrets.h` setup as the earlier networked examples ([example-02](../example-02/README.md#one-time-configuration)). The PIO library `links2004/WebSockets@^2.7.3` is declared in `platformio.ini` and downloaded on first build.

### What's new in this example

- **`WebSocketsServer webSocket(81)`** — a second server alongside the HTTP one, on port 81. Two servers, two ports, one chip.
- **`onWebSocketEvent(num, type, payload, length)`** — library calls this on every WebSocket event. We `switch` on `type`:
  - `WStype_CONNECTED` — a client opened the socket; log its IP.
  - `WStype_DISCONNECTED` — the client went away.
  - `WStype_TEXT` — a text message arrived; first byte `'1'` / `'0'` toggles `LED1status`.
- **`webSocket.begin()` + `webSocket.onEvent(...)`** in `setup()` — start the server and bind the handler.
- **`webSocket.loop()` in `loop()`** — must be called frequently so the library can deliver events to the callback.

The page at `/` is just `<h2>Primer s spletnim vtičnikom WebSocket.</h2>` — no buttons, no JS. That's deliberate: this example is about understanding the server. The client UI is example-11's job.

### What's in `data/`

- `index.html` — the bare-bones heading served at `/`. Confirms the chip is alive. That's the whole UI for this example.

### Build, upload, monitor

```bash
pio run --target upload       # firmware
pio run --target uploadfs     # data/ → LittleFS
pio device monitor            # 115200 baud
```

### How to test it (since there's no UI on `/` yet)

Pick whichever you prefer:

- **Browser dev tools** — open `http://192.168.x.x/` in Chrome/Firefox, F12 → Console, then:
  ```js
  const ws = new WebSocket('ws://' + location.hostname + ':81/');
  ws.onopen = () => console.log('connected');
  ws.send('1');   // LED on
  ws.send('0');   // LED off
  ```
- **`wscat`** on the command line:
  ```bash
  wscat -c ws://192.168.x.x:81
  > 1
  > 0
  ```

In all cases the serial monitor should log the connection and each received message:

```
[0] Klient povezan z IP: 192.168.x.y
[0] Prejeto sporočilo: 1
```

### Troubleshooting

- **WebSocket fails to open** → verify the chip serial shows `WebSocket strežnik je zagnan na vratih 81.`, and that the client is on the same WiFi.
- **Connection succeeds but LED doesn't toggle** → check browser console / `wscat` is sending the literal character `1` / `0`, not the number; the code looks at `payload[0]`.
- **`LittleFS mount failed`** → run `pio run --target uploadfs`.

---

## Slovensko

### Predpogoji

Enaka strojna oprema in nastavitev `secrets.h` kot v prejšnjih omrežnih primerih ([example-02](../example-02/README.md#enkratna-nastavitev)). PIO knjižnica `links2004/WebSockets@^2.7.3` je določena v `platformio.ini` in se prenese ob prvi gradnji.

### Kaj je novega v tem primeru

- **`WebSocketsServer webSocket(81)`** — drugi strežnik poleg HTTP-jevega, na vratih 81. Dva strežnika, dvoja vrata, en čip.
- **`onWebSocketEvent(num, type, payload, length)`** — knjižnica ga pokliče ob vsakem dogodku. V `switch`-u obravnavamo `type`:
  - `WStype_CONNECTED` — odjemalec je odprl vtičnik; izpišemo njegov IP.
  - `WStype_DISCONNECTED` — odjemalec se je odklopil.
  - `WStype_TEXT` — prispelo je besedilno sporočilo; prvi bajt `'1'` / `'0'` preklopi `LED1status`.
- **`webSocket.begin()` + `webSocket.onEvent(...)`** v `setup()` — zaženemo strežnik in vežemo funkcijo.
- **`webSocket.loop()` v `loop()`** — moramo neprestano klicati, da knjižnica dostavi dogodke v funkcijo.

Stran na `/` je samo `<h2>Primer s spletnim vtičnikom WebSocket.</h2>` — brez gumbov, brez JS. Tako je namerno: ta primer razlaga strežnik. Vmesnik je naloga example-11.

### Kaj je v `data/`

- `index.html` — golo besedilo, postreženo na `/`. Potrjuje, da je čip živ. To je ves vmesnik tega primera.

### Prevajanje, nalaganje, spremljanje

```bash
pio run --target upload       # firmware
pio run --target uploadfs     # data/ → LittleFS
pio device monitor            # 115200 baud
```

### Kako preizkusiti (ker na `/` ni vmesnika)

Izberi, kar ti je lažje:

- **Razvojna orodja v brskalniku** — odpri `http://192.168.x.x/` v Chromu/Firefoxu, F12 → Konzola, nato:
  ```js
  const ws = new WebSocket('ws://' + location.hostname + ':81/');
  ws.onopen = () => console.log('connected');
  ws.send('1');   // LED prižge
  ws.send('0');   // LED ugasne
  ```
- **`wscat`** v ukazni vrstici:
  ```bash
  wscat -c ws://192.168.x.x:81
  > 1
  > 0
  ```

V vseh primerih naj serijski monitor zabeleži priklop in vsako prejeto sporočilo:

```
[0] Klient povezan z IP: 192.168.x.y
[0] Prejeto sporočilo: 1
```

### Odpravljanje težav

- **WebSocket se ne odpre** → preveri, da serijski monitor čipa izpiše `WebSocket strežnik je zagnan na vratih 81.`, in da je odjemalec na istem WiFi-ju.
- **Povezava uspe, LED se ne preklopi** → preveri, da brskalnikova konzola / `wscat` pošilja dobesedni znak `1` / `0`, ne številke; koda gleda `payload[0]`.
- **`LittleFS mount failed`** → zaženi `pio run --target uploadfs`.
