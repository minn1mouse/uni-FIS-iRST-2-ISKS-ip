# Node example 01 — HTTP server bound to a specific LAN IP

Same idea as [node-example-00](../node-example-00/) — a tiny Node.js HTTP server with no dependencies — but this one **binds explicitly to a LAN IP** rather than listening on every interface. Useful when you want to be deliberate about which network face the server appears on (e.g. only the wired LAN, not the WiFi adapter), and useful pedagogically to show the difference between binding to `0.0.0.0` (everywhere) and binding to a specific address.

Enaka ideja kot v [node-example-00](../node-example-00/) — drobcen Node.js HTTP strežnik brez odvisnosti — vendar se ta **izrecno veže na IP v lokalnem omrežju** namesto da bi poslušal na vseh vmesnikih. Uporabno, kadar želiš določiti, na katerem omrežnem vmesniku strežnik nastopa (npr. samo žična LAN, ne WiFi), in didaktično pomembno za razliko med vezavo na `0.0.0.0` (povsod) in na specifičen naslov.

---

## English

### Prerequisites

- Node.js (LTS 18+).
- `package.json` with `"type": "module"` is in the folder — needed for the `import` syntax.
- The `hostname` constant in the script **must match your machine's actual LAN IP** for `listen()` to succeed. Default: `192.168.0.145`. Edit it if your laptop's IP is different — find yours with `ipconfig` on Windows.

### Run

From inside `node-example-01/`:

```powershell
node node-example-01.js
```

(or `npm start`.) Expected output:

```
Strežnik teče na http://192.168.0.145:8080
```

### Test it

The URL in the log line is exactly the address to use in a browser, **including the port**:

```
http://192.168.0.145:8080/
```

Reachable from the ESP32, your phone, or any other device on the same LAN. **Not reachable via `localhost`** with this script — because we bound to a specific LAN IP, `127.0.0.1` doesn't match the bind address, so requests to `http://localhost:8080/` will be refused. That's the trade-off of explicit binding.

### Troubleshooting

- **`Error: listen EADDRNOTAVAIL`** → the `hostname` constant doesn't match any IP currently assigned to your machine. Either set it to a real LAN IP from `ipconfig`, or change it to `"0.0.0.0"` to bind to every interface.
- **`Error: listen EADDRINUSE`** → port 8080 is already used. Stop the other process or pick a different port.
- **`SyntaxError: Cannot use import statement outside a module`** → run from inside `node-example-01/` so the `package.json` is picked up.

---

## Slovensko

### Predpogoji

- Node.js (LTS 18+).
- V mapi je `package.json` z `"type": "module"` — potreben za sintakso `import`.
- Konstanta `hostname` v skripti se **mora ujemati z dejanskim LAN IP-jem tvojega računalnika**, sicer `listen()` ne bo uspel. Privzeto: `192.168.0.145`. Spremeni, če ima tvoj prenosnik drugačen IP — preveriš ga z `ipconfig` na Windows.

### Zagon

Iz mape `node-example-01/`:

```powershell
node node-example-01.js
```

(ali `npm start`.) Pričakovan izpis:

```
Strežnik teče na http://192.168.0.145:8080
```

### Preizkus

URL v izpisani vrstici je natanko naslov za uporabo v brskalniku, **vključno z vrati**:

```
http://192.168.0.145:8080/
```

Dosegljivo z ESP32, telefona ali katerekoli druge naprave v istem LAN-u. **Ni dosegljivo prek `localhost`** s to skripto — ker smo se vezali na določen LAN IP, se `127.0.0.1` ne ujema z naslovom vezave, zato bo zahteva na `http://localhost:8080/` zavrnjena. To je cena eksplicitne vezave.

### Odpravljanje težav

- **`Error: listen EADDRNOTAVAIL`** → `hostname` se ne ujema z nobenim trenutno dodeljenim IP-jem računalnika. Nastavi ga na pravi LAN IP iz `ipconfig` ali ga spremeni v `"0.0.0.0"` za vezavo na vse vmesnike.
- **`Error: listen EADDRINUSE`** → vrata 8080 so že zasedena. Ustavi drug proces ali izberi druga vrata.
- **`SyntaxError: Cannot use import statement outside a module`** → zaženi iz mape `node-example-01/`, da se `package.json` upošteva.
