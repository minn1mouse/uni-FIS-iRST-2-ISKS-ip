# Node example 05 — Express + a parallel WebSocket server

Builds on [node-example-04](../node-example-04/) (Express + `ip` auto-discovery) by adding a second server on a second port: a **WebSocket server** via the `ws` package. The browser loads `index` over plain HTTP (port 8080) and then opens a persistent WebSocket back to the server (port 8888). This is the server-side mirror of ESP32-side [example-10](../example-10/README.md) — two servers, two ports, same chip/process.

This example **only sets up the plumbing**: the server logs each new browser connection. Sending and receiving actual messages over the socket is the job of the next example.

Nadgradnja [node-example-04](../node-example-04/) (Express + `ip` samodejno odkrivanje) z drugim strežnikom na drugih vratih: **WebSocket strežnik** prek paketa `ws`. Brskalnik naloži stran po navadnem HTTP (vrata 8080), nato pa odpre še trajno WebSocket povezavo nazaj na strežnik (vrata 8888). To je strežniška različica ESP32 primera [example-10](../example-10/README.md) — dva strežnika, dvoja vrata, isti proces.

V tem primeru **postavljamo le infrastrukturo**: strežnik beleži vsako novo brskalniško povezavo. Pošiljanje in sprejemanje sporočil prek vtičnika je naloga naslednjega primera.

---

## English

### Prerequisites

- Node.js (LTS 18+).
- **One-time setup: install dependencies.** From inside `node-example-05/`:
  ```powershell
  npm install
  ```
  Pulls down `express`, `ip`, and (new in this example) `ws`.

### What's new vs node-example-04

- **`import { WebSocketServer } from "ws"`** — the `ws` package, the de-facto WebSocket library for Node. Same package my homework-03 server stub used.
- **Two servers on two ports.**
  - HTTP (Express) on `HTTP_PORT = 8080`.
  - WebSocket (`ws`) on `WS_PORT = 8888`.
- **`new WebSocketServer({ port: WS_PORT })`** — starts the WebSocket listener.
- **`wss.on("connection", (ws, req) => …)`** — callback fires once per client that opens the socket. Right now it only logs; later it'll also wire up message handlers.
- **Client-side `new WebSocket('ws://${location.hostname}:8888')`** in the HTML — opens the socket on page load. No buttons, no message exchange yet; the connection just stays open silently.

### Run

```powershell
npm install     # only the first time, or after pulling new deps
npm start       # equivalent to: node node-example-05.js
```

Expected output (your IP will differ):

```
HTTP strežnik na http://192.168.0.145:8080/
WebSocket strežnik na ws://192.168.0.145:8888/
```

### Try it

1. Open `http://<your-ip>:8080/` in a browser on the same LAN.
2. The page shows the placeholder heading.
3. **Watch the Node terminal**: a second after the page loads you should see:
   ```
   Brskalnik se je povezal na WebSocket (vrata 8888).
   ```
4. Reload the page → that line appears again. Close the tab → no new line, but no disconnect handler is wired up here (you'll add one in the next example).

### Troubleshooting

- **Page loads but no `Brskalnik se je povezal` line** → check the browser's DevTools → Console for WebSocket errors. Common causes: Windows Defender blocking inbound 8888 (allow Node), or the page is being accessed by IP that doesn't match what the browser uses for `ws://${location.hostname}` (e.g. you visited via `localhost` but Defender is blocking the loopback-to-LAN-IP roundtrip).
- **`Cannot find module 'ws'`** → skipped `npm install`. Run it.
- **`EADDRINUSE: 8888`** → something else is using the WebSocket port. Either stop that process or change `WS_PORT`.
- **`EACCES: permission denied 8080`** → only an issue if you flipped `HTTP_PORT` back to 80 without admin. Bump to 8080.

---

## Slovensko

### Predpogoji

- Node.js (LTS 18+).
- **Enkratna nastavitev: namesti odvisnosti.** Iz mape `node-example-05/`:
  ```powershell
  npm install
  ```
  Povleče `express`, `ip` in (novost) `ws`.

### Kaj je novega v primerjavi z node-example-04

- **`import { WebSocketServer } from "ws"`** — paket `ws`, najpogostejša WebSocket knjižnica za Node. Isti paket kot v zgledu strežnika za homework-03.
- **Dva strežnika na dveh vratih.**
  - HTTP (Express) na `HTTP_PORT = 8080`.
  - WebSocket (`ws`) na `WS_PORT = 8888`.
- **`new WebSocketServer({ port: WS_PORT })`** — zažene poslušanje na WebSocket.
- **`wss.on("connection", (ws, req) => …)`** — povratni klic se sproži ob vsakem odjemalcu, ki odpre vtičnik. Trenutno samo izpiše; kasneje boš tu povezala tudi obravnavo sporočil.
- **Na strani odjemalca `new WebSocket('ws://${location.hostname}:8888')`** v HTML — odpre vtičnik ob nalaganju strani. Brez gumbov, brez izmenjave sporočil; povezava ostane tiho odprta.

### Zagon

```powershell
npm install     # samo prvič, ali po prevzemu novih odvisnosti
npm start       # enako kot: node node-example-05.js
```

Pričakovan izpis (tvoj IP bo drugačen):

```
HTTP strežnik na http://192.168.0.145:8080/
WebSocket strežnik na ws://192.168.0.145:8888/
```

### Preizkus

1. V brskalniku na istem LAN-u odpri `http://<tvoj-ip>:8080/`.
2. Stran prikaže pozdrav.
3. **Spremljaj Node terminal**: sekundo po nalaganju strani bi se moralo izpisati:
   ```
   Brskalnik se je povezal na WebSocket (vrata 8888).
   ```
4. Osveži stran → vrstica se spet izpiše. Zapri zavihek → nove vrstice ni, niti ni vezana obravnava odklopa (to dodaš v naslednjem primeru).

### Odpravljanje težav

- **Stran se naloži, vendar vrstice `Brskalnik se je povezal` ni** → preveri konzolo brskalnika (F12) za napake WebSocket. Pogosti vzroki: požarni zid blokira vhodne povezave na 8888 (dovoli Node), ali pa si stran odprla prek `localhost`, brskalnik pa `location.hostname` razreši nazaj na LAN IP, ki ni dosegljiv prek loopback-a.
- **`Cannot find module 'ws'`** → preskočila si `npm install`. Zaženi ga.
- **`EADDRINUSE: 8888`** → drug proces že uporablja WebSocket vrata. Ustavi ga ali spremeni `WS_PORT`.
- **`EACCES: permission denied 8080`** → samo problem, če si `HTTP_PORT` spremenila nazaj na 80 brez skrbniških pravic. Spremeni na 8080.
