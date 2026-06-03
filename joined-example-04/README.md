# Joined example 04 — Live potentiometer stream to the browser

The ESP32 reads an analog potentiometer on **GPIO 34** at ~20 Hz and pushes every reading as a JSON message through the Node hub to the browser. The browser displays the latest value live, no clicks needed. This is the natural next step after [example-12](../example-12/) (the same ADC reading, but printed to the serial monitor instead of streamed over the network).

ESP32 bere analogni potenciometer na **GPIO 34** s frekvenco ~20 Hz in vsako meritev kot JSON sporočilo pošlje prek Node zvezdišča v brskalnik. Brskalnik najnovejšo vrednost prikazuje v živo, brez klikov. Naraven nadaljevalni korak po [example-12](../example-12/) (enaka ADC meritev, le da je izpisana na serijski monitor namesto poslana po omrežju).

---

## English

### Hardware

- ESP32 dev board on USB.
- A **10 kΩ potentiometer**: wiper → **GPIO 34**, one outer pin → **3.3 V**, other outer pin → **GND**. Without both outer pins connected the ADC reading sits at 0 — same gotcha as example-12.
- Laptop on the same WiFi as the chip, running Node.

### Folder layout

```
joined-example-04/
├── platformio.ini          ← ESP32 build config (WebSockets lib only — no ArduinoJson here)
├── src/main.cpp            ← reads ADC, streams JSON over WebSocket
├── node-example-12/        ← Node side
│   ├── node-example-12.js    ← Express + two WSS + handlePotenciometer router
│   ├── node-example-12.html  ← Browser UI (live value display)
│   └── package.json
└── README.md
```

### What's new vs joined-example-03/03a

- **Sensor streaming (chip side).** Instead of edge-detecting a discrete button, the chip continuously samples an **analog** input and pushes a fresh JSON every 50 ms. New `tipSporočila` value: `"potenciometer"`.
- **Higher message rate.** ~20 Hz instead of "once per button press". Node side intentionally **does not log every message** — at 20 Hz it would flood the terminal. Only unknown/malformed payloads get logged.
- **Browser as live read-out.** `ws.onmessage` updates a value on every incoming message — same hook as joined-03a, just firing much more often. Next step would be plotting on the canvas you introduced in joined-03a.
- **ArduinoJson dropped from `platformio.ini`.** The chip produces JSON (formats it from scratch with a raw-string literal + `String` concatenation) but doesn't consume any — no parser needed.

### Wire format

Single new message type, going chip → browser:

```json
{ "tipSporočila": "potenciometer", "pin": 34, "vrednost": 2048 }
```

`vrednost` is the raw 12-bit ADC value (0–4095). The existing `"LED"` route from joined-02/03 is kept in the Node router so you can layer a button into the HTML if you want browser → chip control too — works out of the box, no Node change required.

### Running

**1. Node hub.** From `joined-example-04/node-example-12/`:

```powershell
npm install
npm start
```

**2. ESP32.** From `joined-example-04/`:

```powershell
pio run --target upload
pio device monitor          # optional
```

**3. Browser:** open `http://<laptop-ip>/`. The page shows two placeholder dashes until the chip connects, then both lines start updating as you turn the knob.

### Troubleshooting

- **Browser values stuck at `—`** → chip hasn't connected to Node yet, or browser hasn't connected. Both connection lines should appear in the Node terminal. Reload the browser, reset the chip.
- **All readings show 0** → no potentiometer wired, or only the wiper is wired without 3.3 V/GND on the outer pins. Same fix as example-12.
- **Values jitter wildly** → either ADC noise (normal — the ESP32's ADC isn't very clean; add a 0.1 µF cap between wiper and GND if it bothers you), or the wiper is floating (loose connection).
- **`EACCES: permission denied 0.0.0.0:80`** → port 80 needs admin on Windows. Run PowerShell as admin or change `HTTP_PORT` to 8080.

### Going further

The pieces are all in place to plot live sensor data:

- The browser already receives a value every 50 ms.
- joined-03a already showed how to draw to a `<canvas>` from a message handler.

Pulling them together: keep a rolling buffer of the last N values in JS, redraw the canvas as a line graph on each message. Minimum viable strip-chart in ~20 lines of additional code.

---

## Slovensko

### Strojna oprema

- ESP32 razvojna ploščica na USB.
- **10 kΩ potenciometer**: drsnik → **GPIO 34**, en zunanji pin → **3.3 V**, drugi zunanji pin → **GND**. Brez obeh zunanjih priključkov ADC bere 0 — enako pasti kot v example-12.
- Prenosnik na istem WiFi-ju kot čip, ki poganja Node.

### Razporeditev map

```
joined-example-04/
├── platformio.ini          ← nastavitve ESP32 (samo WebSockets — brez ArduinoJson)
├── src/main.cpp            ← bere ADC, pošilja JSON prek WebSocket
├── node-example-12/        ← Node stran
│   ├── node-example-12.js    ← Express + dva WSS + usmerjevalnik handlePotenciometer
│   ├── node-example-12.html  ← brskalniški vmesnik (živi prikaz vrednosti)
│   └── package.json
└── README.md
```

### Kaj je novega v primerjavi z joined-example-03/03a

- **Pretočni senzor (stran čipa).** Namesto zaznave roba diskretne tipke čip neprestano vzorči **analogni** vhod in vsakih 50 ms pošlje nov JSON. Nova vrednost `tipSporočila`: `"potenciometer"`.
- **Višja frekvenca sporočil.** ~20 Hz namesto "ob vsakem pritisku tipke". Node namenoma **ne beleži vsakega sporočila** — pri 20 Hz bi zasul terminal. Beleži le neznana/napačna sporočila.
- **Brskalnik kot živi prikaz.** `ws.onmessage` posodablja vrednost ob vsakem prejetem sporočilu — enak vzorec kot v joined-03a, le da se sproži veliko bolj pogosto. Naslednji korak bi bil izris na platno, ki si ga uvedla v joined-03a.
- **ArduinoJson odstranjen iz `platformio.ini`.** Čip JSON le proizvaja (sestavi ga s surovim nizom + `String` združevanjem), ne porablja — razčlenjevalnika ne potrebujemo.

### Oblika sporočila

Ena nova vrsta sporočila, smer čip → brskalnik:

```json
{ "tipSporočila": "potenciometer", "pin": 34, "vrednost": 2048 }
```

`vrednost` je surova 12-bitna vrednost ADC-ja (0–4095). Obstoječa pot `"LED"` iz joined-02/03 je v Node usmerjevalniku ohranjena, tako da lahko v HTML dodaš gumb, če želiš nadzor brskalnik → čip — deluje brez sprememb v Node.

### Zagon

**1. Node zvezdišče.** V mapi `joined-example-04/node-example-12/`:

```powershell
npm install
npm start
```

**2. ESP32.** Iz mape `joined-example-04/`:

```powershell
pio run --target upload
pio device monitor          # neobvezno
```

**3. Brskalnik:** odpri `http://<IP-prenosnika>/`. Stran kaže dva pomišljaja, dokler se čip ne poveže, nato obe vrstici začneta posodabljati vrednosti, ko obračaš gumb potenciometra.

### Odpravljanje težav

- **Vrednosti v brskalniku obstanejo na `—`** → čip se še ni povezal na Node, ali brskalnik se ni povezal. V Node terminalu bi se morali pojaviti obe vrstici o povezavi. Osveži brskalnik, resetiraj čip.
- **Vse meritve so 0** → potenciometer ni ožičen, ali pa je ožičen samo drsnik brez 3.3 V/GND na zunanjih pinih. Enaka rešitev kot v example-12.
- **Vrednosti močno nihajo** → bodisi šum ADC-ja (običajno — ESP32 ADC ni zelo čist; med drsnik in GND vstavi kondenzator 0.1 µF, če te moti), bodisi drsnik lebdi (slab kontakt).
- **`EACCES: permission denied 0.0.0.0:80`** → vrata 80 na Windowsih potrebujejo skrbniške pravice. Zaženi PowerShell kot skrbnik ali spremeni `HTTP_PORT` na 8080.

### Korak naprej

Vsi gradniki za izris meritev v živo so na voljo:

- Brskalnik že prejema vrednost vsakih 50 ms.
- joined-03a je pokazal, kako risati na `<canvas>` iz funkcije za sporočila.

Združitev: hrani drsno okno zadnjih N vrednosti v JS-ju, ob vsakem sporočilu nariši platno kot grafikon. Najmanjši dejaven trakasti graf v ~20 dodatnih vrsticah kode.
