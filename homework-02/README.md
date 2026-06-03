# Homework 02 — Three-LED HTTP control, single-message UI

Variant of [homework-01](../homework-01/README.md). Same three-LED HTTP-driven control idea, but a simpler page: six tiny `/0`–`/5` button-links in a row, plus a single `<h2>` line below that changes per request (a confirmation message saying what just happened). No live state display — clicking re-renders the same page with the latest message.

Različica [homework-01](../homework-01/README.md). Enaka ideja HTTP-krmiljenih treh LED diod, a preprostejša stran: šest majhnih gumbov-povezav `/0`–`/5` v vrsti in ena `<h2>` vrstica pod njimi, ki se spreminja ob vsaki zahtevi (potrditveno sporočilo, kaj se je pravkar zgodilo). Brez prikaza trenutnega stanja — vsak klik vrne isto stran z najnovejšim sporočilom.

---

## English

### Hardware

- ESP32 dev board (`esp32dev`).
- **External LEDs**: red on **GPIO 4**, green on **GPIO 19**. Wire each via a current-limiting resistor (≈220 Ω) to GND. The onboard blue LED is on **GPIO 2** as usual. (Different pin choices from homework-01 — keep this in mind if you swap between the two.)
- A laptop/phone on the same WiFi as the ESP32.

### Prerequisites

Same `secrets.h` setup as the earlier networked examples ([example-02](../example-02/README.md#one-time-configuration)).

### Route map

| URL | Effect | Confirmation message |
| --- | --- | --- |
| `/`  | render help text | "Pritisni gumb (/0 do /5)…" |
| `/0` | blue OFF  | "Prejet ukaz za izklop modre LED diode." |
| `/1` | blue ON   | "Prejet ukaz za vklop modre LED diode." |
| `/2` | red OFF   | "Prejet ukaz za izklop rdece LED diode." |
| `/3` | red ON    | "Prejet ukaz za vklop rdece LED diode." |
| `/4` | green OFF | "Prejet ukaz za izklop zelene LED diode." |
| `/5` | green ON  | "Prejet ukaz za vklop zelene LED diode." |

### What's new vs homework-01

Nothing structurally — same multi-LED + multi-route + LittleFS + template-substitution pattern. The differences are surface-level:

- **Single placeholder.** `data/index.html` has one `%MESSAGE%` token (vs homework-01's six `%BLUE%` / `%BLUE_CLASS%` / etc). Handlers pass the per-route text into `sendPage(...)`.
- **No live state in the page.** Homework-01's page shows each LED's current on/off colour-coded label; here you only see the message from your *last* click.
- **Pin assignments differ** — red is on GPIO 4 (not 5), green is on GPIO 19 (not 18).

### Build, upload, monitor

From inside `homework-02/`:

```bash
pio run                       # compile
pio run --target upload       # flash the code
pio run --target uploadfs     # flash data/ as LittleFS
pio device monitor            # 115200 baud
```

You have to run **both** uploads at least once.

### Expected behaviour

Serial monitor:

```
LittleFS priklopljen.
Povezovanje na home omrežje: TP-Link_1C01
Povezovanje z WiFi omrežjem...
Povezava z WiFi omrežjem je vzpostavljena.
Moj IP naslov je: 192.168.x.x
```

Open `http://<esp32-ip>/` in a browser on the same WiFi. You see the title, a row of six blue button-links (`/0` through `/5`), and the help message below. Click any button → page re-renders with the per-route confirmation message, and the corresponding LED toggles.

### Troubleshooting

- **`LittleFS mount failed`** → run `pio run --target uploadfs`.
- **Page renders the literal `%MESSAGE%`** → the `page.replace("%MESSAGE%", message)` call didn't run, or the placeholder name was edited in only one of `index.html` / `main.cpp`. Check both match.
- **`Manjka /index.html`** in the browser → the filesystem mounted but `index.html` isn't on it. Re-run `uploadfs`.
- **External LED doesn't light** → check wiring polarity and resistor; verify the GPIO can drive an LED (4 and 19 are both fine).

---

## Slovensko

### Strojna oprema

- Razvojna ploščica ESP32 (`esp32dev`).
- **Zunanji LED diodi**: rdeča na **GPIO 4**, zelena na **GPIO 19**. Vsako poveži prek omejevalnega upora (≈220 Ω) na GND. Vgrajena modra LED je kot običajno na **GPIO 2**. (Drugačni pini kot v homework-01 — bodi pozorna pri menjavi med obema.)
- Prenosnik/telefon na istem WiFi-ju kot ESP32.

### Predpogoji

Enaka nastavitev `secrets.h` kot v prejšnjih omrežnih primerih ([example-02](../example-02/README.md#enkratna-nastavitev)).

### Tabela poti

| URL | Učinek | Potrditveno sporočilo |
| --- | --- | --- |
| `/`  | prikaže pomoč | "Pritisni gumb (/0 do /5)…" |
| `/0` | modra OFF  | "Prejet ukaz za izklop modre LED diode." |
| `/1` | modra ON   | "Prejet ukaz za vklop modre LED diode." |
| `/2` | rdeča OFF  | "Prejet ukaz za izklop rdece LED diode." |
| `/3` | rdeča ON   | "Prejet ukaz za vklop rdece LED diode." |
| `/4` | zelena OFF | "Prejet ukaz za izklop zelene LED diode." |
| `/5` | zelena ON  | "Prejet ukaz za vklop zelene LED diode." |

### Kaj je drugače kot v homework-01

Po zgradbi nič — enak večelementni LED + večpotni + LittleFS + vzorec predloge. Razlike so na površju:

- **Ena sama oznaka.** `data/index.html` ima eno oznako `%MESSAGE%` (proti šestim v homework-01: `%BLUE%` / `%BLUE_CLASS%` itd.). Funkcije podajajo besedilo za posamezno pot v `sendPage(...)`.
- **Brez prikaza trenutnega stanja na strani.** Stran homework-01 prikazuje barvno označeno trenutno stanje vsake LED; tu vidiš samo sporočilo od *zadnjega* klika.
- **Drugačni pini** — rdeča je na GPIO 4 (ne 5), zelena na GPIO 19 (ne 18).

### Prevajanje, nalaganje, spremljanje

Iz mape `homework-02/`:

```bash
pio run                       # prevedi
pio run --target upload       # naloži kodo
pio run --target uploadfs     # naloži data/ kot LittleFS
pio device monitor            # 115200 baud
```

**Oba** nalaganja je treba zagnati vsaj enkrat.

### Pričakovano obnašanje

Serijski monitor:

```
LittleFS priklopljen.
Povezovanje na home omrežje: TP-Link_1C01
Povezovanje z WiFi omrežjem...
Povezava z WiFi omrežjem je vzpostavljena.
Moj IP naslov je: 192.168.x.x
```

V brskalniku na istem WiFi-ju odpri `http://<esp32-ip>/`. Vidiš naslov, vrsto šestih modrih gumbov-povezav (`/0` do `/5`) in pomožno sporočilo pod njimi. Klikni katerikoli gumb → stran se ponovno izriše z ustreznim potrditvenim sporočilom, pripadajoča LED pa preklopi.

### Odpravljanje težav

- **`LittleFS mount failed`** → zaženi `pio run --target uploadfs`.
- **Stran prikaže dobesedno `%MESSAGE%`** → klic `page.replace("%MESSAGE%", message)` se ni izvedel, ali pa je oznaka preimenovana le v eni od datotek `index.html` / `main.cpp`. Preveri, da se ujemata.
- **`Manjka /index.html`** v brskalniku → datotečni sistem se je priklopil, vendar `index.html` ni na njem. Ponovno zaženi `uploadfs`.
- **Zunanja LED ne sveti** → preveri polariteto ožičenja in upor; preveri, da pin lahko krmili LED (4 in 19 sta oba primerna).
