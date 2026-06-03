# Joined example 04a — Live line graph of potentiometer values

Identical chip + Node code to [joined-example-04](../joined-example-04/). The **only** difference is in the browser HTML: instead of just printing the latest value, the page now **plots every incoming reading on an HTML5 canvas as a live line graph**. Same rolling-buffer idea joined-04's README suggested as a "going further" exercise — implemented as a small `Graf` JavaScript class.

Enaka koda na čipu in Node-u kot v [joined-example-04](../joined-example-04/). **Edina** razlika je v HTML strani brskalnika: namesto da bi le izpisovala zadnjo vrednost, stran zdaj **vsako prejeto meritev izrisuje na HTML5 platno kot živi linijski graf**. Enaka ideja drsnega medpomnilnika, kot jo je README datoteke joined-04 predlagal kot "korak naprej" — uresničena z drobcenim JS razredom `Graf`.

---

## English

### Hardware

Same as [joined-example-04](../joined-example-04/) — ESP32 + potentiometer wiper on **GPIO 34** + outer pins on **3.3 V**/**GND**.

### Folder layout

```
joined-example-04a/
├── platformio.ini          ← same as joined-04 (WebSockets only)
├── src/main.cpp            ← unchanged from joined-04 (only the header comment is updated)
├── node-example-13/        ← Node side
│   ├── node-example-13.js    ← same router as joined-04's node-example-12
│   ├── node-example-13.html  ← NEW: canvas + `Graf` class for live plotting
│   └── package.json
└── README.md
```

### What's new vs joined-example-04

Only the HTML. Three additions:

1. **`<canvas id="platno1" width="400" height="100">`** — a 400-px-wide drawing surface.
2. **`class Graf`** — a small "strip-chart" object that owns:
   - The canvas's 2D context (`this.plat1`).
   - A rolling `y` buffer of values (max length = canvas width in pixels).
   - A precomputed `x` array `[0, 1, 2, …, width-1]` — one column per pixel, so the time axis advances 1 px per message.
   - Two methods: `dodajVrednostAliBrišiInDodaj(y)` (FIFO append) and `izriši(y)` (append + clear + stroke a polyline through every stored point).
3. **`graf1.izriši(msg.vrednost)` in the `ws.onmessage`** `potenciometer` branch — every incoming reading appends a point and redraws.

The y-scaling: `0..4095` → `0..canvas.height`, then inverted (`canvas.height - y/4095 * canvas.height`) because canvas y-coordinates grow downward.

At the chip's 20 Hz, that gives 20 px/s of horizontal scroll, so the 400 px canvas shows the last ~20 seconds of history. Want a longer window? Make the canvas wider, or sample less often (bump the `delay(50)` in the chip).

### Running

Same procedure as joined-04 — Node first, then the chip, then open the page.

```powershell
# from joined-example-04a/node-example-13/
npm install
npm start

# from joined-example-04a/
pio run --target upload
pio device monitor          # optional
```

Open `http://<laptop-ip>/`. Twist the potentiometer — the red trace should follow.

### Troubleshooting

- **Canvas stays blank** → either the chip isn't streaming (check Node terminal for the `ESP32 se je povezal` line) or the browser hasn't connected (check for `Brskalnik se je povezal`). Same diagnostic chain as joined-04.
- **Trace is flat / always at the bottom** → potentiometer wiper sits at 0 V. Probably an outer pin of the pot isn't connected. Same fix as example-12.
- **Trace is flat / always at the top** → wiper at 3.3 V. Twist it.
- **Trace draws once, then never updates** → `ws.onmessage` fired on the connect message but isn't being called again. Look in DevTools Console for JS errors thrown inside `izriši` that would break the handler on subsequent calls.

### Going further

The `Graf` class works for any single scalar stream. To extend:

- **More streams:** instantiate `graf1 = new Graf("platno1")`, `graf2 = new Graf("platno2")`, route different `tipSporočila` to different graphs.
- **Multi-trace on one canvas:** add a second y-buffer + second `strokeStyle` + a second `lineTo` loop inside `izriši`.
- **Axis labels / gridlines:** add `fillText` and `moveTo`/`lineTo` calls in `izriši` before the main polyline.

---

## Slovensko

### Strojna oprema

Enako kot v [joined-example-04](../joined-example-04/) — ESP32 + drsnik potenciometra na **GPIO 34** + zunanji pini na **3.3 V**/**GND**.

### Razporeditev map

```
joined-example-04a/
├── platformio.ini          ← enako kot joined-04 (samo WebSockets)
├── src/main.cpp            ← nespremenjeno glede na joined-04 (le glava komentarja)
├── node-example-13/        ← Node stran
│   ├── node-example-13.js    ← enak usmerjevalnik kot v joined-04 node-example-12
│   ├── node-example-13.html  ← NOVO: platno + razred `Graf` za živi izris
│   └── package.json
└── README.md
```

### Kaj je novega v primerjavi z joined-example-04

Samo HTML. Tri dodatki:

1. **`<canvas id="platno1" width="400" height="100">`** — risalna površina širine 400 px.
2. **`class Graf`** — drobcen "trakasti" objekt, ki vsebuje:
   - 2D kontekst platna (`this.plat1`).
   - Drsni medpomnilnik `y` vrednosti (največja dolžina = širina platna v pikslih).
   - Vnaprej izračunano tabelo `x` `[0, 1, 2, …, width-1]` — en stolpec na piksel, časovna os napreduje 1 px na sporočilo.
   - Dve metodi: `dodajVrednostAliBrišiInDodaj(y)` (FIFO dodajanje) in `izriši(y)` (dodajanje + brisanje + lomljenka skozi vse točke).
3. **`graf1.izriši(msg.vrednost)` v veji `potenciometer` znotraj `ws.onmessage`** — vsako prejeto meritev doda točko in ponovno nariše.

Skaliranje po y: `0..4095` → `0..canvas.height`, nato obrnjeno (`canvas.height - y/4095 * canvas.height`), ker y-koordinate platna rastejo navzdol.

Pri 20 Hz čipa to da 20 px/s vodoravnega drsenja, zato 400 px široko platno prikaže zadnjih ~20 sekund. Daljše okno? Razširi platno ali vzorči manj pogosto (povečaj `delay(50)` na čipu).

### Zagon

Enak postopek kot v joined-04 — najprej Node, nato čip, nato odpri stran.

```powershell
# iz joined-example-04a/node-example-13/
npm install
npm start

# iz joined-example-04a/
pio run --target upload
pio device monitor          # neobvezno
```

Odpri `http://<IP-prenosnika>/`. Obrni potenciometer — rdeča sled bi morala slediti.

### Odpravljanje težav

- **Platno ostane prazno** → ali čip ne pošilja (preveri vrstico `ESP32 se je povezal` v Node terminalu) ali se brskalnik ni povezal (preveri `Brskalnik se je povezal`). Enako kot v joined-04.
- **Sled je ravna / vedno spodaj** → drsnik potenciometra je na 0 V. Verjetno zunanji pin ni priključen. Enaka rešitev kot v example-12.
- **Sled je ravna / vedno zgoraj** → drsnik na 3.3 V. Zavrti ga.
- **Sled se nariše enkrat in se nikoli več ne osveži** → `ws.onmessage` se je sprožil ob priklopu, a se ne kliče več. V DevTools konzoli poišči JS napake znotraj `izriši`, ki bi prekinile delovanje za naslednje klice.

### Korak naprej

Razred `Graf` deluje za poljuben skalarni tok. Razširitve:

- **Več tokov:** ustvari `graf1 = new Graf("platno1")`, `graf2 = new Graf("platno2")`, različne `tipSporočila` usmeri v različne grafe.
- **Več sledi na enem platnu:** dodaj drug y-medpomnilnik + drug `strokeStyle` + drugo zanko `lineTo` znotraj `izriši`.
- **Oznake osi / mreža:** v `izriši` pred glavno lomljenko dodaj klice `fillText` in `moveTo`/`lineTo`.
