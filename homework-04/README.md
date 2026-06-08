# Homework 04 — Live sensor streaming with a browser graph

This homework has **no code of its own**. The deliverable is one chip-only ADC demo plus two `joined-example-*` projects, which together cover the assignment's three stages — from a raw analog read on the serial monitor to a live line-graph in the browser, driven by the same chip code.

Ta domača naloga **nima svoje kode**. Oddaja so en samostojen ADC primer na čipu ter dva `joined-example-*` projekta, ki skupaj pokrijejo tri stopnje naloge — od surovega branja analognega vhoda na serijskem monitoru do živega grafa v brskalniku, ki ga poganja ista koda na čipu.

---

## English

### What to look at

| Example | Stage | What it demonstrates |
|---|---|---|
| [example-12](../example-12/) | Stage 1 | Bare ADC read. Chip-only — no network. Potentiometer on GPIO 34, value printed to the serial monitor every 50 ms. Establishes `analogRead` and the 0–4095 range. |
| [joined-example-04](../joined-example-04/) | Stage 2 | Same ADC read, but streamed over WebSocket through the Node hub to a browser. New `tipSporočila`: `"potenciometer"`. Browser displays the latest value live as text. |
| [joined-example-04a](../joined-example-04a/) | Stage 3 | Same chip code as joined-04 — only the browser HTML changes: instead of a text readout, a small `<canvas>` plots a rolling line-graph of the last ~30 s of readings. |

### Architecture (shared with joined-04 and 04a)

```
Browser ── ws://laptop:8888 ──┐
                              ├── Node hub ──── ws://laptop:8811 ── ESP32
Browser ── ws://laptop:8888 ──┘
```

Two `WebSocketServer` instances live in the Node hub: **8888** for browsers, **8811** for the ESP32. A single `handleIncoming(source, raw)` router parses each message's `tipSporočila` and forwards it to the right side. Same hub pattern as every joined example from `joined-example-02` onward.

### Wire format

```json
{ "tipSporočila": "potenciometer", "pin": 34, "vrednost": 2048 }
```

`vrednost` is the raw 12-bit ADC reading (0–4095). Chip-side is producer-only; nothing is sent the other direction in these three examples.

### Running

Each of the three projects has its own README with full build/run instructions. The pattern is the same in every case:

1. From the Node subfolder (only joined-04 / 04a): `npm install` then `npm start`.
2. From the project root: `pio run --target upload`, then optionally `pio device monitor`.
3. Open `http://<laptop-ip>/` in the browser (joined-04 / 04a only).

For example-12 there's no Node side — just monitor the serial console.

---

## Slovensko

### Kaj pogledati

| Primer | Stopnja | Kaj prikazuje |
|---|---|---|
| [example-12](../example-12/) | 1. stopnja | Golo branje ADC-ja. Samo čip — brez omrežja. Potenciometer na GPIO 34, vrednost se na 50 ms izpiše na serijski monitor. Predstavi `analogRead` in območje 0–4095. |
| [joined-example-04](../joined-example-04/) | 2. stopnja | Isto branje ADC-ja, a poslano prek WebSocket-a in Node zvezdišča v brskalnik. Nova vrsta `tipSporočila`: `"potenciometer"`. Brskalnik v živo prikazuje zadnjo vrednost kot besedilo. |
| [joined-example-04a](../joined-example-04a/) | 3. stopnja | Enaka koda na čipu kot v joined-04 — spremeni se le HTML v brskalniku: namesto izpisa vrednosti se na manjšem `<canvas>` izriše drsni linijski graf zadnjih ~30 s meritev. |

### Arhitektura (skupna z joined-04 in 04a)

```
Brskalnik ── ws://prenosnik:8888 ──┐
                                   ├── Node zvezdišče ── ws://prenosnik:8811 ── ESP32
Brskalnik ── ws://prenosnik:8888 ──┘
```

V Node zvezdišču tečeta dva `WebSocketServer`-ja: **8888** za brskalnike, **8811** za ESP32. Skupni usmerjevalnik `handleIncoming(source, raw)` razčleni `tipSporočila` vsakega sporočila in ga posreduje na pravo stran. Enak vzorec zvezdišča kot v vseh joined primerih od `joined-example-02` naprej.

### Oblika sporočil

```json
{ "tipSporočila": "potenciometer", "pin": 34, "vrednost": 2048 }
```

`vrednost` je surova 12-bitna vrednost ADC-ja (0–4095). Čip je tu le proizvajalec; v teh treh primerih se v nasprotno smer ne pošilja nič.

### Zagon

Vsak od treh projektov ima svoj README s polnimi navodili za prevajanje in zagon. Vzorec je vedno enak:

1. V Node podmapi (samo joined-04 / 04a): `npm install`, nato `npm start`.
2. V korenski mapi projekta: `pio run --target upload`, po želji `pio device monitor`.
3. V brskalniku odpri `http://<IP-prenosnika>/` (samo joined-04 / 04a).

Za example-12 ni Node strani — dovolj je spremljanje serijske konzole.
