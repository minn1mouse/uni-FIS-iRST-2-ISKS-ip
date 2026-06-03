# Joined example 04 — Visualise button events on an HTML5 canvas

Identical chip + Node code to [joined-example-03](../joined-example-03/). The **only** difference is the browser HTML: in addition to the text line that shows the latest button JSON, the page now paints an HTML5 `<canvas>` — pink when the button is pressed, white when released. Same bidirectional flow, same JSON protocol, same Node router. The lesson is purely on the front-end: how to draw to a canvas in response to a stream of WebSocket events.

Enaka koda na čipu in Node-u kot v [joined-example-03](../joined-example-03/). **Edina** razlika je HTML strani brskalnika: poleg besedilne vrstice, ki kaže zadnji JSON tipke, stran zdaj barva HTML5 `<canvas>` — rožnato ob pritisku, belo ob spustu. Enaka dvosmerna povezava, enak JSON protokol, enak Node usmerjevalnik. Lekcija je čisto na strani brskalnika: kako risati na platno v odziv na tok WebSocket dogodkov.

---

## English

### Hardware

Same as [joined-example-03](../joined-example-03/) — ESP32 + push button on GPIO 18 + onboard LED on GPIO 2 + a laptop on the same WiFi.

### Folder layout

```
joined-example-04/
├── platformio.ini          ← unchanged from joined-03
├── src/main.cpp            ← unchanged from joined-03 (only the header comment updated)
├── node-example-11/        ← Node side
│   ├── node-example-11.js    ← unchanged in shape from joined-03's node-example-10.js
│   ├── node-example-11.html  ← NEW: HTML5 canvas painted from button events
│   └── package.json
└── README.md
```

### What's new vs joined-example-03

Only in `node-example-11.html`. Three additions:

1. **`<canvas id="platno1" width="200" height="100" style="border: 1px dashed black"></canvas>`** — a fixed-size drawing surface.
2. **Get the 2D drawing context once at script start:**
   ```js
   let plat    = document.getElementById("platno1");
   let platno1 = plat.getContext("2d");
   ```
   `platno1` is now an object with `fillStyle`, `fillRect`, `strokeStyle`, `arc`, etc. — the whole 2D drawing API.
3. **In `ws.onmessage`'s `tipka` branch**, paint the canvas based on `msg.vrednost`:
   ```js
   platno1.fillStyle = (msg.vrednost == 1) ? "lightpink" : "white";
   platno1.fillRect(0, 0, 200, 100);
   ```
   `fillRect(x, y, w, h)` fills a rectangle in the current `fillStyle` — here it covers the whole canvas (0,0 to 200×100), effectively repainting the background.

Everything else (browser → ESP32 LED control, ESP32 → browser button events, the text label, the Node router) is verbatim from joined-03.

### Running both sides

Same procedure as joined-03 — Node first (`npm start` in `node-example-11/`), then upload + monitor the chip, then open the browser page.

```powershell
# from joined-example-04/node-example-11/
npm install
npm start

# from joined-example-04/
pio run --target upload
pio device monitor
```

### Trying it

- **LED path (browser → chip):** click *Pošlji JSON sporočilo* with the default JSON — LED on. Edit `"vrednost":0`, click again — LED off.
- **Canvas path (chip → browser):** press the GPIO 18 button. The page:
  - updates the *Zadnje sporočilo z ESP32* line with the JSON, and
  - **fills the canvas pink**.
  Release the button — canvas turns white again.

### Why a canvas at all?

For this example, the canvas does what a CSS background colour change would also do — it's overkill. The point is **the API**, not the visual: once you have `getContext("2d")` and you're already inside a message handler, you can draw anything that streams in from the ESP32. Same hook also lets you draw:

- A live plot of analogue sensor readings (call `lineTo(x, y)` for each new sample).
- A 2D position indicator (clear and redraw a circle every time the chip pushes new x/y).
- A bar chart of recent values.

So this example is the smallest possible "ESP32 stream → live visual" demo. Replace the fillRect with anything you'd draw and the rest of the pipeline still works.

---

## Slovensko

### Strojna oprema

Enako kot v [joined-example-03](../joined-example-03/) — ESP32 + tipka na GPIO 18 + vgrajena LED na GPIO 2 + prenosnik na istem WiFi-ju.

### Razporeditev map

```
joined-example-04/
├── platformio.ini          ← nespremenjeno iz joined-03
├── src/main.cpp            ← nespremenjeno iz joined-03 (le glava komentarja je posodobljena)
├── node-example-11/        ← Node stran
│   ├── node-example-11.js    ← po obliki nespremenjeno glede na joined-03 node-example-10.js
│   ├── node-example-11.html  ← NOVO: HTML5 platno, barva se na dogodke tipke
│   └── package.json
└── README.md
```

### Kaj je novega v primerjavi z joined-example-03

Samo v `node-example-11.html`. Tri dodaje:

1. **`<canvas id="platno1" width="200" height="100" style="border: 1px dashed black"></canvas>`** — risalna površina fiksne velikosti.
2. **2D kontekst risanja dobimo enkrat na začetku skripte:**
   ```js
   let plat    = document.getElementById("platno1");
   let platno1 = plat.getContext("2d");
   ```
   `platno1` je odslej objekt z `fillStyle`, `fillRect`, `strokeStyle`, `arc` itd. — celoten 2D risarski API.
3. **V `ws.onmessage` veji za `tipka`** obarvamo platno glede na `msg.vrednost`:
   ```js
   platno1.fillStyle = (msg.vrednost == 1) ? "lightpink" : "white";
   platno1.fillRect(0, 0, 200, 100);
   ```
   `fillRect(x, y, w, h)` zapolni pravokotnik s trenutnim `fillStyle` — tukaj pokrije celotno platno (0,0 do 200×100), kar v praksi ponovno pobarva ozadje.

Vse drugo (brskalnik → ESP32 nadzor LED, ESP32 → brskalnik dogodki tipke, besedilna oznaka, Node usmerjevalnik) je dobesedno iz joined-03.

### Zagon obeh strani

Enak postopek kot v joined-03 — najprej Node (`npm start` v mapi `node-example-11/`), nato nalaganje + monitor čipa, nato odpri stran v brskalniku.

```powershell
# iz joined-example-04/node-example-11/
npm install
npm start

# iz joined-example-04/
pio run --target upload
pio device monitor
```

### Preizkus

- **Pot LED (brskalnik → čip):** klikni *Pošlji JSON sporočilo* s privzetim JSON-om — LED se prižge. Spremeni `"vrednost":0`, znova klikni — LED ugasne.
- **Pot platna (čip → brskalnik):** pritisni tipko na GPIO 18. Stran:
  - posodobi vrstico *Zadnje sporočilo z ESP32* z JSON-om in
  - **platno obarva rožnato**.
  Spusti tipko — platno spet pobeli.

### Zakaj sploh platno?

V tem primeru platno opravi enako, kot bi tudi sprememba CSS ozadja — gre za pretiravanje. Bistvo je **API**, ne vizualni učinek: ko imaš `getContext("2d")` in si že znotraj funkcije za sporočila, lahko narišeš karkoli, kar prihaja iz ESP32. Isto sidro omogoča tudi:

- Živi izris analognih meritev senzorjev (kliči `lineTo(x, y)` ob vsakem novem vzorcu).
- Indikator 2D položaja (počisti in znova nariši krog ob vsakih novih koordinatah čipa).
- Stolpčni grafikon zadnjih vrednosti.

Ta primer je torej najmanjši možen "ESP32 tok → živi vidni prikaz". Zamenjaj fillRect s čimerkoli risarskim in preostala cev še naprej deluje.
