# Homework 03 — Browser ↔ Node ↔ ESP32 over WebSockets

This homework has **no code of its own**. The deliverable is three of the `joined-example-*` projects, which together cover the assignment's three stages — from one-way browser control of an LED to bidirectional messaging with a small canvas visualisation.

Ta domača naloga **nima svoje kode**. Oddaja so trije `joined-example-*` projekti, ki skupaj pokrijejo tri stopnje naloge — od enosmernega upravljanja LED iz brskalnika do dvosmerne izmenjave sporočil z manjšim izrisom na platnu.

---

## English

### What to look at

| Example | Stage | What it demonstrates |
|---|---|---|
| [joined-example-02](../joined-example-02/) | Stage 1 | Browser → Node → ESP32. JSON message turns the onboard LED on/off. Chip is a WebSocket **client**. |
| [joined-example-03](../joined-example-03/) | Stage 2 | Adds the reverse direction. The chip's button (GPIO 18) pushes `tipka` events to all connected browsers, which print the latest state. |
| [joined-example-03a](../joined-example-03a/) | Stage 3 | Same chip code as 03 — only the browser HTML changes: instead of plain text, a small canvas draws a coloured square that follows the button state. |

### Architecture (shared across all three)

```
Browser ── ws://laptop:8888 ──┐
                              ├── Node hub ──── ws://laptop:8811 ── ESP32
Browser ── ws://laptop:8888 ──┘
```

Two `WebSocketServer` instances live in the Node hub: **8888** for browsers, **8811** for the ESP32. A single `handleIncoming(source, raw)` router parses each message's `tipSporočila` and forwards it to the right side. Same hub pattern as every joined example from `joined-example-02` onward.

### Wire format

```json
{ "tipSporočila": "LED",   "pin": 2,  "vrednost": 1 }
{ "tipSporočila": "tipka", "pin": 18, "vrednost": 1 }
```

### Running

Each of the three projects has its own README with full build/run instructions. The pattern is the same in every case:

1. From the Node subfolder: `npm install` then `npm start`.
2. From the project root: `pio run --target upload`, then optionally `pio device monitor`.
3. Open `http://<laptop-ip>/` in the browser.

---

## Slovensko

### Kaj pogledati

| Primer | Stopnja | Kaj prikazuje |
|---|---|---|
| [joined-example-02](../joined-example-02/) | 1. stopnja | Brskalnik → Node → ESP32. JSON sporočilo prižge/ugasne vgrajeno LED. Čip je WebSocket **odjemalec**. |
| [joined-example-03](../joined-example-03/) | 2. stopnja | Doda obratno smer. Tipka na čipu (GPIO 18) pošilja dogodke `tipka` vsem priklopljenim brskalnikom, ki izpišejo zadnje stanje. |
| [joined-example-03a](../joined-example-03a/) | 3. stopnja | Enaka koda na čipu kot v 03 — spremeni se le HTML v brskalniku: namesto golega besedila se na platnu izriše obarvan kvadrat, ki sledi stanju tipke. |

### Arhitektura (skupna vsem trem)

```
Brskalnik ── ws://prenosnik:8888 ──┐
                                   ├── Node zvezdišče ── ws://prenosnik:8811 ── ESP32
Brskalnik ── ws://prenosnik:8888 ──┘
```

V Node zvezdišču tečeta dva `WebSocketServer`-ja: **8888** za brskalnike, **8811** za ESP32. Skupni usmerjevalnik `handleIncoming(source, raw)` razčleni `tipSporočila` vsakega sporočila in ga posreduje na pravo stran. Enak vzorec zvezdišča kot v vseh joined primerih od `joined-example-02` naprej.

### Oblika sporočil

```json
{ "tipSporočila": "LED",   "pin": 2,  "vrednost": 1 }
{ "tipSporočila": "tipka", "pin": 18, "vrednost": 1 }
```

### Zagon

Vsak od treh projektov ima svoj README s polnimi navodili za prevajanje in zagon. Vzorec je vedno enak:

1. V Node podmapi: `npm install`, nato `npm start`.
2. V korenski mapi projekta: `pio run --target upload`, po želji `pio device monitor`.
3. V brskalniku odpri `http://<IP-prenosnika>/`.
