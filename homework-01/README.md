# Homework 01 — Three-LED HTTP control with templated HTML

A single styled web page that shows the live state of three LEDs (onboard blue + external red on GPIO 5 + external green on GPIO 18) and lets you switch each one on or off with a button. The page lives as a real `.html` file in `data/`, served from LittleFS like [example-05c](../example-05c/), with placeholders (`%BLUE%`, `%RED%`, `%GREEN%` and friends) substituted at request time to reflect the current state.

Ena slogovno oblikovana spletna stran, ki kaže trenutno stanje treh LED diod (vgrajene modre + zunanje rdeče na GPIO 5 + zunanje zelene na GPIO 18) in omogoča preklop vsake z gumbom. Stran je prava `.html` datoteka v `data/`, postrežena iz LittleFS kot v [example-05c](../example-05c/), z oznakami (`%BLUE%`, `%RED%`, `%GREEN%` itd.), ki se ob vsaki zahtevi zamenjajo s trenutnim stanjem.

---

## English

### Hardware

- ESP32 dev board (`esp32dev`).
- **External LEDs**: red on **GPIO 5**, green on **GPIO 18**. Wire each via a current-limiting resistor (≈220 Ω) to GND. The onboard blue LED is on **GPIO 2** as usual.
- A laptop/phone on the same WiFi for the browser client.

### Prerequisites

Same `secrets.h` setup as the earlier networked examples ([example-02](../example-02/README.md#one-time-configuration)).

### What's new in this homework

- **Three LEDs, three states.** `blueOn` / `redOn` / `greenOn` are independent globals; the main loop drives each pin from its own flag.
- **Templated HTML, served from LittleFS.** `data/index.html` carries the page (with CSS) and placeholder tokens like `%BLUE%` and `%BLUE_CLASS%`. The handler reads the file, runs `page.replace(...)` for each placeholder, and sends the result. This is a tiny server-side templating pattern good for a handful of dynamic values.
- **Seven routes**:
  | URL | Effect |
  | --- | --- |
  | `/`  | render current state |
  | `/0` | blue OFF  |
  | `/1` | blue ON   |
  | `/2` | red OFF   |
  | `/3` | red ON    |
  | `/4` | green OFF |
  | `/5` | green ON  |
  Each route handler updates the corresponding flag, then calls `sendPage()` so the page reflects the new state immediately.

### Build, upload, monitor

From inside `homework-01/`:

```bash
pio run                       # compile the code
pio run --target upload       # flash the code
pio run --target uploadfs     # flash the data/ folder as LittleFS
pio device monitor            # 115200 baud
```

You have to run **both** uploads at least once (`upload` for the firmware, `uploadfs` for the HTML). After that, code edits need only `upload`, HTML edits need only `uploadfs`.

### Expected behaviour

Serial monitor:

```
LittleFS priklopljen.
Povezovanje na home omrežje: TP-Link_1C01
Povezovanje z WiFi omrežjem ...
Povezava z WiFi omrežjem je vzpostavljena.
Moj IP naslov je: 192.168.x.x
```

Then in a browser on the same WiFi, open `http://<esp32-ip>/`. You see three rows (Modra / Rdeča / Zelena LED), each showing its current state (**PRIŽGANA** green / **UGASNJENA** red) with **Prižgi** and **Ugasni** buttons that swap the state and re-render.

### Troubleshooting

- **`LittleFS mount failed`** → run `pio run --target uploadfs`.
- **Page shows literal `%BLUE%` etc.** → `String::replace` didn't run (or template tokens were renamed). Check the `page.replace(...)` calls in `sendPage()` cover every token in `index.html`.
- **`Manjka /index.html`** in the browser → the filesystem mounted but `index.html` isn't on it. Re-run `uploadfs`.
- **External LED doesn't light** → check wiring polarity and that the GPIO can drive an LED (5 and 18 can; some pins are strap-only).

---

## Slovensko

### Strojna oprema

- Razvojna ploščica ESP32 (`esp32dev`).
- **Zunanji LED diodi**: rdeča na **GPIO 5**, zelena na **GPIO 18**. Vsako poveži prek omejevalnega upora (≈220 Ω) na GND. Vgrajena modra LED je kot običajno na **GPIO 2**.
- Prenosnik/telefon na istem WiFi-ju za odjemalca v brskalniku.

### Predpogoji

Enaka nastavitev `secrets.h` kot v prejšnjih omrežnih primerih ([example-02](../example-02/README.md#enkratna-nastavitev)).

### Kaj je novega v tej domači nalogi

- **Tri LED, tri stanja.** `blueOn` / `redOn` / `greenOn` so neodvisne globalne spremenljivke; glavna zanka vsako nožico krmili glede na svojo zastavico.
- **HTML predloga, postrežena iz LittleFS.** `data/index.html` nosi stran (s CSS-jem) in oznake kot `%BLUE%` in `%BLUE_CLASS%`. Funkcija prebere datoteko, za vsako oznako zažene `page.replace(...)` in pošlje rezultat. Drobcen vzorec strežniške predloge, primeren za nekaj dinamičnih vrednosti.
- **Sedem poti**:
  | URL | Učinek |
  | --- | --- |
  | `/`  | prikaže trenutno stanje |
  | `/0` | modra OFF  |
  | `/1` | modra ON   |
  | `/2` | rdeča OFF  |
  | `/3` | rdeča ON   |
  | `/4` | zelena OFF |
  | `/5` | zelena ON  |
  Vsaka funkcija posodobi ustrezno zastavico in nato pokliče `sendPage()`, tako da stran takoj odraža novo stanje.

### Prevajanje, nalaganje, spremljanje

Iz mape `homework-01/`:

```bash
pio run                       # prevedi kodo
pio run --target upload       # naloži kodo
pio run --target uploadfs     # naloži mapo data/ kot LittleFS
pio device monitor            # 115200 baud
```

**Oba** nalaganja je treba zagnati vsaj enkrat (`upload` za kodo, `uploadfs` za HTML). Po tem ponovni `upload` osveži le kodo, ponovni `uploadfs` le HTML.

### Pričakovano obnašanje

Serijski monitor:

```
LittleFS priklopljen.
Povezovanje na home omrežje: TP-Link_1C01
Povezovanje z WiFi omrežjem ...
Povezava z WiFi omrežjem je vzpostavljena.
Moj IP naslov je: 192.168.x.x
```

Nato v brskalniku na istem WiFi-ju odpri `http://<esp32-ip>/`. Vidiš tri vrstice (Modra / Rdeča / Zelena LED), vsaka kaže trenutno stanje (**PRIŽGANA** zelena / **UGASNJENA** rdeča) z gumboma **Prižgi** in **Ugasni**, ki preklopita stanje in stran ponovno izrišeta.

### Odpravljanje težav

- **`LittleFS mount failed`** → zaženi `pio run --target uploadfs`.
- **Stran prikaže dobesedno `%BLUE%` ipd.** → `String::replace` se ni izvedel (ali pa so oznake preimenovane). Preveri, da `page.replace(...)` klici v `sendPage()` pokrivajo vsako oznako iz `index.html`.
- **`Manjka /index.html`** v brskalniku → datotečni sistem se je priklopil, vendar `index.html` ni na njem. Ponovno zaženi `uploadfs`.
- **Zunanja LED ne sveti** → preveri polariteto ožičenja in da pin lahko krmili LED (5 in 18 lahko; nekateri pini so samo strap).
