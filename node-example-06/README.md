# Node example 06 — WebSocket with a real client UI (browser → server)

Builds on [node-example-05](../node-example-05/) by adding **actual message exchange** in **one direction** (browser → server). The HTML now has a button + input field; typing something and clicking sends a WebSocket message to Node, which logs it. The server-to-browser direction comes in the next example.

Same code shape as the ESP32-side jump from [example-10](../example-10/) → [example-11](../example-11/) — bare WebSocket scaffold, then add the UX layer.

Nadgradnja [node-example-05](../node-example-05/) z dejansko **izmenjavo sporočil** v **eni smeri** (brskalnik → strežnik). HTML zdaj vsebuje gumb in vnosno polje; ko nekaj vpišeš in klikneš, se WebSocket sporočilo pošlje na Node, ta pa ga zabeleži. Smer strežnik → brskalnik pride v naslednjem primeru.

Enak vzorec kot prehod na strani ESP32 [example-10](../example-10/) → [example-11](../example-11/) — gola WebSocket infrastruktura, nato dodan UX.

---

## English

### Prerequisites

- Node.js (LTS 18+).
- **One-time setup:** `npm install` inside `node-example-06/` (pulls `express`, `ip`, `ws` — same set as example-05).
- HTTP server listens on **port 80** — needs an elevated PowerShell on Windows. Drop `HTTP_PORT` to 8080 in the script if you'd rather run without admin.

### What's new vs node-example-05

- **In the HTML:** button + input + an `onclick` handler that reads the input value and calls `ws.send(value)`.
- **In the JS server:** inside the `wss.on("connection", ...)` callback we now also register `ws.on("message", (msg) => ...)`. That's the per-client message handler — once per browser, fires for every message that browser pushes.
- Everything else (Express + `ip.address()` + `WebSocketServer` on a parallel port) is unchanged from example-05.

### Run

```powershell
npm install     # only the first time
npm start       # node node-example-06.js
```

### Try it

1. Open `http://<your-ip>/` in a browser on the same LAN. You see the heading, a button, and an input prefilled with `Testno sporočilo!`.
2. Click **Pošlji sporočilo**. The Node terminal logs:
   ```
   Prejeto sporočilo: Testno sporočilo!
   ```
3. Edit the input field and click again — each click pushes the new value over the open socket. No page reload between messages; the socket stays connected the whole time.

### Browser dev tools alternative

Want to skip the button and send raw messages? F12 → Console, then:

```js
ws.send("kar koli")
```

`ws` is in scope from the `<script>` block. The server logs whatever you send.

### Troubleshooting

- **Button does nothing / no log line on the server** → F12 → Console. Likely a JS error (typo in `pošljiSporočilo`) or the WebSocket failed to open (look for a red `ws://...` line in DevTools → Network).
- **`Cannot find module 'ws'`** → skipped `npm install`.
- **`EACCES: permission denied 80`** → HTTP_PORT is 80; either run PowerShell as admin or drop to 8080.

---

## Slovensko

### Predpogoji

- Node.js (LTS 18+).
- **Enkratna nastavitev:** `npm install` v mapi `node-example-06/` (povleče `express`, `ip`, `ws` — enako kot v example-05).
- HTTP strežnik posluša na **vratih 80** — na Windowsih potrebuje skrbniški PowerShell. Spremeni `HTTP_PORT` na 8080 v skripti, če želiš teči brez skrbniških pravic.

### Kaj je novega v primerjavi z node-example-05

- **V HTML-ju:** gumb + vnosno polje + funkcija `onclick`, ki prebere vrednost vnosnega polja in pokliče `ws.send(value)`.
- **V JS strežniku:** znotraj povratnega klica `wss.on("connection", ...)` zdaj registriramo še `ws.on("message", (msg) => ...)`. To je funkcija za sporočila tega odjemalca — registrira se enkrat na brskalnik, sproži pa za vsako sporočilo, ki ga ta brskalnik pošlje.
- Vse drugo (Express + `ip.address()` + `WebSocketServer` na vzporednih vratih) je enako kot v example-05.

### Zagon

```powershell
npm install     # samo prvič
npm start       # node node-example-06.js
```

### Preizkus

1. V brskalniku na istem LAN-u odpri `http://<tvoj-ip>/`. Vidiš naslov, gumb in vnosno polje, vnaprej napolnjeno s `Testno sporočilo!`.
2. Klikni **Pošlji sporočilo**. Node terminal zabeleži:
   ```
   Prejeto sporočilo: Testno sporočilo!
   ```
3. Spremeni vrednost v vnosnem polju in klikni znova — vsak klik pošlje novo vrednost prek odprtega vtičnika. Brez osveževanja strani; vtičnik je ves čas povezan.

### Alternativa: razvojna konzola brskalnika

Bi rada preskočila gumb in pošiljala surova sporočila? F12 → Konzola:

```js
ws.send("kar koli")
```

`ws` je v dosegu iz `<script>` bloka. Strežnik zabeleži vse, kar pošlješ.

### Odpravljanje težav

- **Gumb ne dela / na strežniku ni vrstice o sporočilu** → F12 → Konzola. Verjetno JS napaka (tipkarska napaka v `pošljiSporočilo`) ali pa se WebSocket ni odprl (preveri rdečo `ws://...` vrstico v DevTools → Network).
- **`Cannot find module 'ws'`** → preskočila si `npm install`.
- **`EACCES: permission denied 80`** → `HTTP_PORT` je 80; zaženi PowerShell kot skrbnik ali ga spremeni na 8080.
