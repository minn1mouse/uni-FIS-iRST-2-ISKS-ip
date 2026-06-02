# Node example 03 — Multiple routes, each serving its own HTML file

Builds on [node-example-02](../node-example-02/) (single static HTML file via `fs.readFile`). Now the server dispatches by `req.url`: three routes (`/`, `/stranx`, `/strany`) each return a different `.html` from disk. Also introduces the `__dirname` reconstruction pattern for ES modules so the file lookups use absolute paths and work regardless of which directory you run `node` from.

Nadgradnja [node-example-02](../node-example-02/) (ena statična HTML datoteka prek `fs.readFile`). Strežnik zdaj razdeli zahteve po `req.url`: tri poti (`/`, `/stranx`, `/strany`) vsaka vrne drugo `.html` z diska. Predstavi tudi vzorec za rekonstrukcijo `__dirname` v ES modulih, tako da poti do datotek niso več relativne in skripta deluje ne glede na to, iz katerega imenika jo zaženeš.

---

## English

### Prerequisites

- Node.js (LTS 18+).
- `package.json` with `"type": "module"`.
- **`shared/node/config.js`** configured ([see node-example-01](../node-example-01/README.md#prerequisites)).

### What's new vs node-example-02

- **Multiple routes** dispatched in the request handler:
  ```js
  if      (req.url === "/")        serveFile(res, "node-example-03.html");
  else if (req.url === "/stranx")  serveFile(res, "node-example-03x.html");
  else if (req.url === "/strany")  serveFile(res, "node-example-03y.html");
  else                             /* 404 */;
  ```
- **404 fallback** for any path that doesn't match — the lecturer's original had no else branch and would just hang the response. Now you get a clear "Ni take poti" message.
- **`__dirname` reconstruction.** ES modules don't expose `__dirname` / `__filename` like CommonJS does. The recipe:
  ```js
  import { fileURLToPath } from "url";
  import path from "path";
  const __filename = fileURLToPath(import.meta.url);
  const __dirname  = path.dirname(__filename);
  ```
  Then `path.join(__dirname, "foo.html")` is always an absolute path, regardless of the current working directory.
- **`serveFile(res, filename)` helper** — factored out so the three route bodies stay one line each.

### Bug fixes that landed in the refactor

The original code had:

- A hardcoded `hostname = "192.168.1.198"` — wrong subnet for our setup. Now `SERVER_HOST` comes from `shared/node/config.js`.
- File names that didn't match the files on disk. The code read `pr03.html` / `pr03x.html` / `pr03y.html` but the actual files are `node-example-03.html` / `node-example-03x.html` / `node-example-03y.html` — every request would have returned 500. Fixed to point at the real filenames.

### Run

From inside `node-example-03/`:

```powershell
node node-example-03.js
```

### Try it

After the log line appears, in a browser on the same LAN:

| URL | Returns |
| --- | --- |
| `http://<SERVER_HOST>:8080/`        | `node-example-03.html` |
| `http://<SERVER_HOST>:8080/stranx`  | `node-example-03x.html` |
| `http://<SERVER_HOST>:8080/strany`  | `node-example-03y.html` |
| anything else | 404 *"Ni take poti: …"* |

### Troubleshooting

- **`SyntaxError: Cannot use import statement outside a module`** → run from inside `node-example-03/`.
- **`EADDRNOTAVAIL`** → `SERVER_HOST` in `shared/node/config.js` doesn't match any IP on this machine. Update it.
- **Browser shows the 500 page** → the .html file is missing or unreadable. Check that all three exist in `node-example-03/` and the path inside `serveFile` matches the filename on disk.

---

## Slovensko

### Predpogoji

- Node.js (LTS 18+).
- `package.json` z `"type": "module"`.
- Nastavljen **`shared/node/config.js`** ([glej node-example-01](../node-example-01/README.md#predpogoji)).

### Kaj je novega v primerjavi z node-example-02

- **Več poti**, razdeljenih v funkciji zahteve:
  ```js
  if      (req.url === "/")        serveFile(res, "node-example-03.html");
  else if (req.url === "/stranx")  serveFile(res, "node-example-03x.html");
  else if (req.url === "/strany")  serveFile(res, "node-example-03y.html");
  else                             /* 404 */;
  ```
- **404 rezerva** za vsako pot, ki se ne ujema — predavateljev izvirnik je `else` veje ni imel in bi odgovor zataknil. Zdaj dobiš jasno sporočilo "Ni take poti".
- **Rekonstrukcija `__dirname`.** ES moduli ne ponujajo `__dirname` / `__filename` kot CommonJS. Recept:
  ```js
  import { fileURLToPath } from "url";
  import path from "path";
  const __filename = fileURLToPath(import.meta.url);
  const __dirname  = path.dirname(__filename);
  ```
  Nato je `path.join(__dirname, "foo.html")` vedno absolutna pot, ne glede na trenutni delovni imenik.
- **Pomožna funkcija `serveFile(res, filename)`** — izvlečena, da telesi treh poti ostaneta vsako po eno vrstico.

### Popravki napak, ki so se zgodili ob refaktoriranju

V izvirniku je bil:

- Trdo kodiran `hostname = "192.168.1.198"` — napačno omrežje za našo nastavitev. Zdaj `SERVER_HOST` prihaja iz `shared/node/config.js`.
- Imena datotek se niso ujemala s tistimi na disku. Koda je brala `pr03.html` / `pr03x.html` / `pr03y.html`, a dejanske datoteke so `node-example-03.html` / `node-example-03x.html` / `node-example-03y.html` — vsaka zahteva bi vrnila 500. Popravljeno na prava imena.

### Zagon

Iz mape `node-example-03/`:

```powershell
node node-example-03.js
```

### Preizkus

Ko se izpiše vrstica o teku strežnika, v brskalniku na istem LAN-u:

| URL | Vrne |
| --- | --- |
| `http://<SERVER_HOST>:8080/`        | `node-example-03.html` |
| `http://<SERVER_HOST>:8080/stranx`  | `node-example-03x.html` |
| `http://<SERVER_HOST>:8080/strany`  | `node-example-03y.html` |
| karkoli drugega | 404 *"Ni take poti: …"* |

### Odpravljanje težav

- **`SyntaxError: Cannot use import statement outside a module`** → zaženi iz mape `node-example-03/`.
- **`EADDRNOTAVAIL`** → `SERVER_HOST` v `shared/node/config.js` se ne ujema z nobenim IP-jem na tem računalniku. Posodobi ga.
- **Brskalnik prikaže 500 stran** → .html datoteka manjka ali ni berljiva. Preveri, da vse tri obstajajo v `node-example-03/` in da se ime v `serveFile` ujema z imenom na disku.
