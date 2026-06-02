# Node example 00 — Minimal HTTP "Hello world" server

Smallest possible Node.js HTTP server: ~5 lines, no dependencies, just the built-in `http` module. Listens on every network interface at port 8080 and responds with the same plain-text greeting to every request.

Najmanjši možen Node.js HTTP strežnik: ~5 vrstic, brez odvisnosti, samo vgrajen modul `http`. Posluša na vseh omrežnih vmesnikih na vratih 8080 in vsaki zahtevi odgovori z istim navadnim pozdravom.

---

## English

### Prerequisites

- Node.js installed (any reasonably modern LTS — 18 or newer). Verify with `node --version`.
- The folder ships with a tiny `package.json` whose only purpose is to set `"type": "module"` so `import http from 'http'` works.

### Run

From inside `node-example-00/`:

```powershell
node node-example-00.js
```

(or `npm start` — both invoke the same command.)

You should see:

```
Server running on http://0.0.0.0:8080
```

### Test it

In a browser on the same machine:

```
http://localhost:8080/
```

From the ESP32 / another device on the LAN, use the laptop's IP instead of `localhost`, e.g. `http://192.168.0.145:8080/`. Every URL (path, query, method) returns the same plain text:

```
Pozdravljen svet
```

### Stop it

`Ctrl+C` in the terminal.

### Troubleshooting

- **`SyntaxError: Cannot use import statement outside a module`** → the `package.json` with `"type": "module"` is missing or you're running the file from outside this folder. Run from inside `node-example-00/`.
- **`EADDRINUSE` on port 8080** → something else is using the port. Either stop that service or change `.listen(8080)` to a different port.
- **Browser shows "site can't be reached" from another device** → Windows Firewall is blocking inbound 8080. Allow Node.js through Defender (Private networks).

---

## Slovensko

### Predpogoji

- Nameščen Node.js (kakršen koli sodobno LTS — 18 ali novejši). Preveri z `node --version`.
- V mapi je drobcen `package.json`, ki služi samo za nastavitev `"type": "module"`, da `import http from 'http'` deluje.

### Zagon

Iz mape `node-example-00/`:

```powershell
node node-example-00.js
```

(ali `npm start` — oba ukaza poženeta isto.)

Pokaže se:

```
Server running on http://0.0.0.0:8080
```

### Preizkus

V brskalniku na istem računalniku:

```
http://localhost:8080/
```

Z ESP32 ali drugo napravo v lokalnem omrežju uporabi IP prenosnika namesto `localhost`, npr. `http://192.168.0.145:8080/`. Vsak URL (pot, parametri, metoda) vrne isto navadno besedilo:

```
Pozdravljen svet
```

### Ustavitev

`Ctrl+C` v terminalu.

### Odpravljanje težav

- **`SyntaxError: Cannot use import statement outside a module`** → manjka `package.json` z `"type": "module"`, ali pa datoteko poganjaš zunaj te mape. Poženi iz mape `node-example-00/`.
- **`EADDRINUSE` na vratih 8080** → drug proces že uporablja ta vrata. Ustavi tisto storitev ali spremeni `.listen(8080)` na druga vrata.
- **Brskalnik na drugi napravi pravi, da spletna stran ni dosegljiva** → požarni zid Windows blokira vhodne povezave na 8080. Dovoli Node.js v Defenderju (zasebna omrežja).
