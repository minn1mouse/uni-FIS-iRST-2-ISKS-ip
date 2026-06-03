# Node example 04 — Same idea as node-example-03, but with Express

Two new pieces vs [node-example-03](../node-example-03/):

1. **Express.js framework.** Routes and static-file serving collapse from manual `req.url`-switching + `fs.readFile` + `res.writeHead` into one-liners (`app.get(path, handler)` + `res.sendFile(path)`).
2. **`ip` package.** Auto-discovers the laptop's LAN IP at startup, so the printed URL is always right without per-developer config. This example **doesn't** import `shared/node/config.js` — the IP is found automatically.

Dve novosti v primerjavi z [node-example-03](../node-example-03/):

1. **Ogrodje Express.js.** Poti in postrežba statičnih datotek se iz ročnega `req.url`-stikala + `fs.readFile` + `res.writeHead` skrčijo v eno-vrstičnice (`app.get(pot, funkcija)` + `res.sendFile(pot)`).
2. **Paket `ip`.** Ob zagonu samodejno odkrije LAN IP prenosnika, tako da je izpisani URL vedno pravilen — brez per-razvijalske nastavitve. Ta primer **ne** uvaža `shared/node/config.js` — IP najde sam.

---

## English

### Prerequisites

- Node.js (LTS 18+).
- **First-time setup: install the dependencies** declared in `package.json`. From inside `node-example-04/`:
  ```powershell
  npm install
  ```
  That pulls down `express` and `ip` into a local `node_modules/` (gitignored). One-time per example.

### What's new vs node-example-03

- **`import express from "express"`** — install with `npm install express`. Creates an `app` object with chainable route registration.
- **`http.createServer(app)`** — Express's app object isn't itself a server; you wrap it with the built-in `http.createServer` to get a listenable Node server.
- **`app.get("/", (req, res) => …)`** — one line replaces the entire `if (req.url === "/")` block plus the `fs.readFile` callback plus the manual `res.writeHead`. Add as many `app.get(...)` lines as you have routes; Express handles the dispatch.
- **`res.sendFile(absolutePath)`** — streams a file with Content-Type inferred from the extension. No manual headers, no error path to write yourself (Express handles missing-file 404s by default if no other handler matches).
- **`import ip from "ip"; ip.address()`** — returns the machine's LAN IP. Used in the `console.log` so the printed URL always points at whichever network you're on, no editing required.

### Port note

The lecturer's original used port **80** (the default HTTP port). On Windows that requires an elevated PowerShell — otherwise you get `EACCES`. This refactor uses **8080** so it works out of the box; flip it back to 80 in `const PORT = ...` if you're on Linux/macOS or running as admin.

### Run

```powershell
npm install        # only the first time, or after pulling new deps
npm start          # equivalent to: node node-example-04.js
```

Expected output (your IP will differ):

```
Strežnik zagnan na http://192.168.0.145:8080/
```

Open that URL in a browser on the same LAN. You should see:

> Pozdrav iz spletne strani html — uporaba knjižnice express!

### Troubleshooting

- **`Cannot find module 'express'` / `'ip'`** → you skipped `npm install`. Run it from inside this folder.
- **`EACCES: permission denied 0.0.0.0:80`** → you set `PORT` back to 80 without admin. Either bump to 8080 or right-click PowerShell → Run as administrator.
- **`ip.address()` returns `127.0.0.1` or wrong interface** → multi-interface machine (WiFi + Ethernet + VPN). The `ip` package picks one; if it's wrong, swap back to the `SERVER_HOST` from `shared/node/config.js` like node-example-03 does.

---

## Slovensko

### Predpogoji

- Node.js (LTS 18+).
- **Enkratna nastavitev: namesti odvisnosti** iz `package.json`. Iz mape `node-example-04/`:
  ```powershell
  npm install
  ```
  Ukaz povleče `express` in `ip` v lokalno mapo `node_modules/` (gitignored). Enkrat na primer.

### Kaj je novega v primerjavi z node-example-03

- **`import express from "express"`** — namestiš z `npm install express`. Ustvari objekt `app` s tečnim registriranjem poti.
- **`http.createServer(app)`** — sam Express `app` ni strežnik; ovijemo ga z vgrajenim `http.createServer`, da dobimo Node strežnik, na katerem lahko poslušamo.
- **`app.get("/", (req, res) => …)`** — ena vrstica nadomesti cel `if (req.url === "/")` blok skupaj s `fs.readFile` funkcijo in ročnim `res.writeHead`. Dodaš toliko vrstic `app.get(...)`, kolikor poti potrebuješ; Express opravi razdelitev.
- **`res.sendFile(absolutnaPot)`** — pošlje datoteko s Content-Type-om, ki ga samodejno določi iz končnice. Ročnih glav ni, niti poti za napake ti ni treba pisati (Express privzeto obravnava 404 za neobstoječe datoteke, če se ne ujema noben drug rokovalnik).
- **`import ip from "ip"; ip.address()`** — vrne LAN IP računalnika. Uporabljen v `console.log`, tako da izpisani URL vedno kaže na omrežje, na katerem si — brez urejanja.

### Opomba o vratih

Predavateljev izvirnik je uporabljal vrata **80** (privzeta HTTP vrata). Na Windows to zahteva skrbniški PowerShell — sicer dobiš `EACCES`. Ta refaktor uporablja **8080**, da deluje brez priprav; v `const PORT = ...` jih spremeni nazaj na 80, če si na Linuxu/macOS ali poganjaš kot skrbnik.

### Zagon

```powershell
npm install        # samo prvič, ali po prevzemu novih odvisnosti
npm start          # enako kot: node node-example-04.js
```

Pričakovan izpis (tvoj IP bo drugačen):

```
Strežnik zagnan na http://192.168.0.145:8080/
```

URL odpri v brskalniku na istem LAN-u. Pokazati bi se moralo:

> Pozdrav iz spletne strani html — uporaba knjižnice express!

### Odpravljanje težav

- **`Cannot find module 'express'` / `'ip'`** → preskočila si `npm install`. Zaženi ga v tej mapi.
- **`EACCES: permission denied 0.0.0.0:80`** → `PORT` si nastavila nazaj na 80 brez skrbniških pravic. Spremeni na 8080 ali odpri PowerShell kot skrbnik.
- **`ip.address()` vrne `127.0.0.1` ali napačen vmesnik** → računalnik z več vmesniki (WiFi + Ethernet + VPN). Paket `ip` izbere enega; če izbere napačnega, preklopi nazaj na `SERVER_HOST` iz `shared/node/config.js`, kot to počne node-example-03.
