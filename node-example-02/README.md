# Node example 02 — Serve a static HTML file from disk

Same idea as [node-example-01](../node-example-01/) — a tiny Node HTTP server bound to your LAN IP — but the response body is no longer an inline string. The server reads `node-example-02.html` from disk on every request with `fs.readFile` and pipes the bytes into the response. First step toward serving real web pages.

Enaka ideja kot v [node-example-01](../node-example-01/) — drobcen Node HTTP strežnik, vezan na LAN IP — vendar telo odgovora ni več vrstični niz. Strežnik ob vsaki zahtevi prebere `node-example-02.html` z diska z `fs.readFile` in pošlje bajte v odgovor. Prvi korak proti postrežbi pravih spletnih strani.

---

## English

### Prerequisites

- Node.js (LTS 18+).
- `package.json` with `"type": "module"`.
- **`shared/node/config.js`** configured ([see node-example-01](../node-example-01/README.md#prerequisites)).

### What's new vs node-example-01

- **`import fs from "fs"`** — Node's built-in filesystem module.
- **`fs.readFile("node-example-02.html", callback)`** — async read of an HTML file relative to the working directory. The callback gets either an error or the file contents.
- **Error path** — if the file is missing or unreadable, respond with `500` and a plain-text error. Otherwise respond with `200` + `text/html` + the file bytes.

Everything else (the binding, the LAN-IP-from-shared-config, the server.listen log line) is identical.

### Run

From inside `node-example-02/`:

```powershell
node node-example-02.js
```

Visit the URL printed in the log; you should see the HTML page render.

### Caveat

`fs.readFile` uses a relative path here, so it resolves against **the directory you ran the command from**. If you run `node node-example-02/node-example-02.js` from the repo root, it'll fail to find the file. Run from inside the folder, or upgrade to the absolute-path pattern shown in [node-example-03](../node-example-03/) (`path.join(__dirname, "…")`).

---

## Slovensko

### Predpogoji

- Node.js (LTS 18+).
- `package.json` z `"type": "module"`.
- Nastavljen **`shared/node/config.js`** ([glej node-example-01](../node-example-01/README.md#predpogoji)).

### Kaj je novega v primerjavi z node-example-01

- **`import fs from "fs"`** — Node vgrajeni modul za delo z datotekami.
- **`fs.readFile("node-example-02.html", callback)`** — asinhrono branje HTML datoteke glede na trenutni delovni imenik. Funkcija povratnega klica prejme napako ali vsebino datoteke.
- **Pot za napake** — če datoteka manjka ali ni berljiva, odgovori s `500` in besedilnim sporočilom. V nasprotnem primeru odgovori s `200` + `text/html` + vsebino datoteke.

Vse drugo (vezava, LAN IP iz skupne konfiguracije, izpis pri zagonu) je enako.

### Zagon

Iz mape `node-example-02/`:

```powershell
node node-example-02.js
```

Odpri URL iz izpisa; pokazati se mora HTML stran.

### Opozorilo

`fs.readFile` tu uporablja relativno pot, ki se razreši glede na **mapo, iz katere si zagnala ukaz**. Če zaženeš `node node-example-02/node-example-02.js` iz korena repozitorija, datoteke ne bo našel. Zaženi iz mape primera ali preidi na vzorec z absolutno potjo iz [node-example-03](../node-example-03/) (`path.join(__dirname, "…")`).
