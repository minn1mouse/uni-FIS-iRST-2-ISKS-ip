# Example 05c — Serve HTML from a flash filesystem (LittleFS)

Same behaviour as [example-05b](../example-05b/README.md) — visit `/1` to turn the onboard LED on, `/0` to turn it off — but the HTML pages now live as **real `.html` files in a `data/` folder** and are written to a small filesystem in the ESP32's flash. The C++ code reads them at runtime instead of carrying inline `PROGMEM` constants.

Enako vedenje kot v [example-05b](../example-05b/README.md) — odpri `/1` za vklop vgrajene LED in `/0` za izklop — vendar so HTML strani zdaj **prave `.html` datoteke v mapi `data/`** in se zapišejo v majhen datotečni sistem v flash pomnilniku ESP32. Koda C++ jih bere v času izvajanja namesto, da bi nosila vrstične `PROGMEM` konstante.

---

## English

### Prerequisites

Same hardware and `secrets.h` setup as the previous examples ([example-02](../example-02/README.md#one-time-configuration)).

### What's new in this example

- **`data/` folder.** Holds real `.html` files (`index.html`, `on.html`, `off.html`) — full editor syntax highlighting, opens in any browser locally for preview, diffs cleanly in git.
- **`board_build.filesystem = littlefs` in `platformio.ini`.** Tells PlatformIO to package the `data/` folder as a LittleFS image instead of the older SPIFFS default.
- **`#include <LittleFS.h>` + `LittleFS.begin()`.** Mounts the filesystem in `setup()`. If the mount fails, you forgot to upload the filesystem — see below.
- **`server.streamFile(f, "text/html; charset=UTF-8")`.** Reads a file and writes it directly to the HTTP response — no need to load the whole thing into RAM first.

### Two upload steps

This is the one real conceptual jump. There are now **two separate things on the chip**:

| What | Lives in | Uploaded by |
| --- | --- | --- |
| Your compiled code | `firmware.bin` | `pio run --target upload` |
| The `data/` folder | `littlefs.bin` | `pio run --target uploadfs` |

You have to run **both** at least once. After that, re-running `upload` only refreshes the code (HTML stays put); re-running `uploadfs` only refreshes the HTML (code stays put).

### Build, upload, monitor

From inside `example-05c/`:

```bash
pio run                       # compile the code
pio run --target upload       # flash the code
pio run --target uploadfs     # flash the data/ folder as LittleFS
pio device monitor            # 115200 baud
```

In VSCode's PlatformIO sidebar: under `example-05c → Project Tasks → esp32dev → Platform`, you'll find both **Upload** (code) and **Upload Filesystem Image** (`uploadfs`).

### Expected behaviour

Serial monitor should show:

```
LittleFS priklopljen.
Povezovanje na home omrežje: TP-Link_1C01
...
Povezava z WiFi omrežjem je vzpostavljena.
Moj IP naslov je: 192.168.x.x
```

…then in a browser on the same WiFi:

| URL | Response | LED |
| --- | --- | --- |
| `http://<esp32-ip>/`  | instruction page from `index.html` | unchanged |
| `http://<esp32-ip>/1` | `on.html` | turns **on** |
| `http://<esp32-ip>/0` | `off.html` | turns **off** |

### Troubleshooting

- **`LittleFS mount failed`** on the serial monitor → run `pio run --target uploadfs`.
- **Every page returns 404 "Datoteka ne obstaja"** → mount succeeded but `data/` was empty during the filesystem build, or you uploaded an older firmware that points at differently-named files. Re-run `uploadfs`.
- **`uploadfs` fails with `Could not open COMx, the port is busy`** → close the serial monitor first (same as for normal upload).
- **Edited an HTML file and the change isn't showing** → HTML files are *only* refreshed by `uploadfs`; plain `upload` won't pick them up.

---

## Slovensko

### Predpogoji

Enaka strojna oprema in nastavitev `secrets.h` kot v prejšnjih primerih ([example-02](../example-02/README.md#enkratna-nastavitev)).

### Kaj je novega v tem primeru

- **Mapa `data/`.** Vsebuje prave `.html` datoteke (`index.html`, `on.html`, `off.html`) — polno označevanje sintakse v urejevalniku, lokalno odprtje v brskalniku za predogled, čisto se vidi v git diff-u.
- **`board_build.filesystem = littlefs` v `platformio.ini`.** PlatformIO-ju pove, naj mapo `data/` zapakira kot LittleFS sliko namesto starejše privzete SPIFFS.
- **`#include <LittleFS.h>` + `LittleFS.begin()`.** Priklopi datotečni sistem v `setup()`. Če priklop ne uspe, si pozabila naložiti datotečni sistem — glej spodaj.
- **`server.streamFile(f, "text/html; charset=UTF-8")`.** Datoteko prebere in jo neposredno zapiše v HTTP odgovor — ni potrebno naložiti celotne vsebine v RAM.

### Dva koraka nalaganja

To je en pravi konceptualni preskok. Zdaj sta na čipu **dve ločeni stvari**:

| Kaj | Živi v | Naloži se z |
| --- | --- | --- |
| Tvoja prevedena koda | `firmware.bin` | `pio run --target upload` |
| Mapa `data/` | `littlefs.bin` | `pio run --target uploadfs` |

**Oba** moraš zagnati vsaj enkrat. Po tem ponovni `upload` osveži le kodo (HTML ostane), ponovni `uploadfs` osveži le HTML (koda ostane).

### Prevajanje, nalaganje, spremljanje

Iz mape `example-05c/`:

```bash
pio run                       # prevedi kodo
pio run --target upload       # naloži kodo
pio run --target uploadfs     # naloži mapo data/ kot LittleFS
pio device monitor            # 115200 baud
```

V VSCode PlatformIO stranski vrstici pod `example-05c → Project Tasks → esp32dev → Platform` najdeš oba: **Upload** (kodo) in **Upload Filesystem Image** (`uploadfs`).

### Pričakovano obnašanje

Serijski monitor naj pokaže:

```
LittleFS priklopljen.
Povezovanje na home omrežje: TP-Link_1C01
...
Povezava z WiFi omrežjem je vzpostavljena.
Moj IP naslov je: 192.168.x.x
```

…nato v brskalniku na istem WiFi-ju:

| URL | Odgovor | LED |
| --- | --- | --- |
| `http://<esp32-ip>/`  | navodilna stran iz `index.html` | brez spremembe |
| `http://<esp32-ip>/1` | `on.html` | **prižge** |
| `http://<esp32-ip>/0` | `off.html` | **ugasne** |

### Odpravljanje težav

- **`LittleFS mount failed`** na serijskem monitorju → zaženi `pio run --target uploadfs`.
- **Vse strani vrnejo 404 "Datoteka ne obstaja"** → priklop je uspel, vendar je bila `data/` prazna med gradnjo datotečnega sistema, ali pa si naložila starejšo verzijo kode z drugačnimi imeni datotek. Ponovno zaženi `uploadfs`.
- **`uploadfs` ne uspe z `Could not open COMx, the port is busy`** → najprej zapri serijski monitor (enako kot pri običajnem `upload`).
- **Urejal-a si HTML, a spremembe ni v brskalniku** → HTML se osveži *samo* z `uploadfs`; navadni `upload` ga ne dotakne.
