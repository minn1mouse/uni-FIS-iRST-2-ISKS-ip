# Node example 07 — Parse incoming WebSocket messages as JSON

Builds on [node-example-06](../node-example-06/) (browser → server message exchange) by treating the payload as **JSON** instead of a raw string. The input field is pre-filled with a JSON-shaped value (`{"tipSporočila": "LED", "pin": 2, "vrednost": 1}`); the server `JSON.parse`s whatever the client sends and logs the structured fields. This is the **same wire format** used by [homework-03](../homework-03/) — except there the ESP32 was the reader and Node was the sender. Here the roles are flipped: the browser sends, Node parses.

Nadgradnja [node-example-06](../node-example-06/) (izmenjava brskalnik → strežnik) z obravnavo vsebine kot **JSON** namesto kot surovega niza. Vnosno polje je vnaprej napolnjeno z JSON sporočilom (`{"tipSporočila": "LED", "pin": 2, "vrednost": 1}`); strežnik z `JSON.parse` razčleni karkoli odjemalec pošlje in zabeleži strukturirana polja. Gre za **isti format kot v [homework-03](../homework-03/)** — le da je tam ESP32 bral, Node pa pošiljal. Tukaj sta vlogi obrnjeni: brskalnik pošlje, Node razčleni.

---

## English

### Prerequisites

- Node.js (LTS 18+).
- **`npm install`** inside `node-example-07/` (deps: `express`, `ip`, `ws` — same set as the previous WebSocket examples).
- HTTP server on **port 80** — admin PowerShell on Windows, or change `HTTP_PORT` to 8080.

### What's new vs node-example-06

- **Input field default value is JSON.** Same shape `homework-03` reads on the ESP32 side:
  ```json
  { "tipSporočila": "LED", "pin": 2, "vrednost": 1 }
  ```
- **Server-side `JSON.parse`** inside the `ws.on("message", ...)` handler turns the incoming string into a JavaScript object, after which `msg.tipSporočila`, `msg.pin`, and `msg.vrednost` are normal field accesses.
- **`try/catch` around the parse** — `JSON.parse` throws on malformed input, so wrapping it prevents one bad message from crashing the whole server.

### Run

```powershell
npm install
npm start       # node node-example-07.js
```

### Try it

1. Open `http://<your-ip>/` in a browser on the same LAN.
2. Click **Pošlji sporočilo** — Node logs:
   ```
   Prejeto sporočilo: {"tipSporočila": "LED", "pin": 2, "vrednost": 1}
     tipSporočila: LED
     pin:          2
     vrednost:     1
   ```
3. Edit the input field — change the JSON values, or break the JSON deliberately. Bad JSON logs `(sporočilo ni veljaven JSON: ...)` and the server keeps running.

### Why JSON?

A WebSocket can carry any string. Plain text works for single values (`"1"`, `"0"`) but breaks down when you need to send multiple fields. JSON gives you a tiny, universally-understood format both Node and Arduino/C++ have first-class parsers for. Pairing this example with `homework-03` is the natural follow-up: a Node program that *generates* JSON and an ESP32 that *consumes* it.

### Troubleshooting

- **`SyntaxError: Unexpected token`** on the server → the input field doesn't contain valid JSON. Reset the value, or use the [JSON validator](https://jsonlint.com/) to check.
- **Page works, server log shows `Prejeto sporočilo` but no parsed fields** → the JSON parsed but the field names don't match. This example expects `tipSporočila`, `pin`, `vrednost` exactly (Slovene names match the lecturer's design and homework-03).
- **`Cannot find module 'ws'`** → skipped `npm install`.

---

## Slovensko

### Predpogoji

- Node.js (LTS 18+).
- **`npm install`** v mapi `node-example-07/` (odvisnosti: `express`, `ip`, `ws` — enako kot v prejšnjih WebSocket primerih).
- HTTP strežnik na **vratih 80** — skrbniški PowerShell na Windowsih ali spremeni `HTTP_PORT` na 8080.

### Kaj je novega v primerjavi z node-example-06

- **Privzeta vrednost vnosnega polja je JSON.** Enaka oblika, kot jo na strani ESP32 bere `homework-03`:
  ```json
  { "tipSporočila": "LED", "pin": 2, "vrednost": 1 }
  ```
- **`JSON.parse` na strani strežnika** znotraj funkcije `ws.on("message", ...)` dohodni niz pretvori v JavaScript objekt, nato pa `msg.tipSporočila`, `msg.pin` in `msg.vrednost` postanejo običajni dostopi do polj.
- **`try/catch` okrog razčlenjevanja** — `JSON.parse` v primeru napačnega vhoda vrže napako, ovijanje preprečuje, da bi eno slabo sporočilo zrušilo strežnik.

### Zagon

```powershell
npm install
npm start       # node node-example-07.js
```

### Preizkus

1. V brskalniku na istem LAN-u odpri `http://<tvoj-ip>/`.
2. Klikni **Pošlji sporočilo** — Node izpiše:
   ```
   Prejeto sporočilo: {"tipSporočila": "LED", "pin": 2, "vrednost": 1}
     tipSporočila: LED
     pin:          2
     vrednost:     1
   ```
3. Spremeni vsebino vnosnega polja — spremeni vrednosti ali namerno pokvari JSON. Napačen JSON izpiše `(sporočilo ni veljaven JSON: ...)`, strežnik pa še naprej teče.

### Zakaj JSON?

WebSocket lahko prenaša poljuben niz. Navadno besedilo deluje za eno vrednost (`"1"`, `"0"`), a se zalomi, ko želiš pošiljati več polj hkrati. JSON ponuja drobcen, splošno razumljen format, za katerega imata Node in Arduino/C++ vgrajena razčlenjevalnika. Naravno nadaljevanje tega primera je sodelovanje s `homework-03`: Node program, ki *ustvarja* JSON, in ESP32, ki ga *prejema*.

### Odpravljanje težav

- **`SyntaxError: Unexpected token`** na strežniku → vsebina vnosnega polja ni veljaven JSON. Resetiraj vrednost ali preveri z [JSON validatorjem](https://jsonlint.com/).
- **Stran deluje, dnevnik izpiše `Prejeto sporočilo`, polj pa ne** → JSON se je razčlenil, vendar se imena polj ne ujemajo. Ta primer pričakuje natanko `tipSporočila`, `pin`, `vrednost` (slovenska imena se ujemajo s predavateljevim načrtom in homework-03).
- **`Cannot find module 'ws'`** → preskočila si `npm install`.
