# Homework 04 — Closed-loop LED control with photoresistor feedback

A control system that verifies its own commands. The browser sends ON/OFF commands for a green LED through a Node.js hub; the ESP32 actuates the LED and uses a photoresistor (LDR) as **feedback** to confirm the light is actually emitting. When the sensor disagrees with the command (LED removed, burned, or covered), a reserve red LED lights up and the onboard blue LED blinks as an alarm — and the UI shows an error banner.

Kontrolni sistem, ki sam preverja svoje ukaze. Brskalnik prek Node.js zvezdišča pošlje ukaze za vklop/izklop zelene LED diode; ESP32 jo krmili in s fotouporniku (LDR) kot **povratno zvezo** potrjuje, da dejansko oddaja svetlobo. Ko se senzor ne strinja z ukazom (LED odstranjena, pregorela ali zakrita), rezervna rdeča LED zasveti, vgrajena modra utripa kot alarm — uporabniški vmesnik pa prikaže pas z napako.

---

## English

### Hardware

- ESP32 dev board on USB.
- **Green LED** on **GPIO 5** — the controlled actuator. Through a current-limiting resistor (≈220 Ω) to GND.
- **Red LED** on **GPIO 21** — reserve actuator; lights up when the comparator detects `napaka_vklop` (commanded ON but sensor reads dark). Same wiring as green.
- **Blue onboard LED** on **GPIO 2** — alarm; blinks while any error state is active. No external wiring.
- **Photoresistor (LDR)** on **GPIO 32** in a voltage divider:
  `3.3 V → LDR → GPIO 32 → 10 kΩ → GND`.
  Bright light = high ADC value, darkness = low ADC value. Position the LDR so it sees the green LED directly (touching is fine).
- Laptop on the same WiFi as the chip, running Node.

### Folder layout

```
homework-04/
├── platformio.ini             ← ESP32 build config (WebSockets + ArduinoJson)
├── src/main.cpp               ← WebSocket client, comparator, error logic
├── node-homework-04/          ← Node side
│   ├── node-homework-04.js      ← Express + two WSS + status cache
│   ├── node-homework-04.html    ← Browser UI (buttons + chart + banner)
│   └── package.json
└── README.md
```

### Control-system structure

```
   intention                                                       
   (setpoint)        error         control                         
       │              │              │                             
       ▼              ▼              ▼                             
   ┌───────┐      ┌───────┐      ┌──────────┐    ┌──────────┐
   │  UI   │─────►│ COMP- │─────►│CONTROLLER│───►│ ACTUATOR │──┐
   │buttons│      │ARATOR │      │  (logic) │    │ green LED│  │
   └───────┘      └───────┘      └──────────┘    └──────────┘  │
                      ▲                                         │
                      │                              light ─────┘
                      │                                ▼
                      │                          ┌──────────┐
                      └──────── feedback ────────│  SENSOR  │
                              (LDR value)        │ LDR + ADC│
                                                 └──────────┘
```

Mapping to control-system vocabulary:

| Role          | Slovene                | In this project                                                  |
|---------------|------------------------|------------------------------------------------------------------|
| Setpoint      | nastavitvena vrednost  | `greenIntention` (true/false), set by button clicks              |
| Comparator    | primerjalnik           | `if/else` in `loop()` classifying ok / napaka_vklop / napaka_izklop |
| Controller    | krmilnik               | Lines after the comparator that drive red/blue and emit `status` |
| Actuator      | izvršni element        | Green LED (and red LED as reserve when actuator fails)            |
| Process       | proces                  | Light propagation from LED to LDR surface                         |
| Sensor        | senzor                  | LDR + voltage divider + 12-bit ADC on GPIO 32                     |
| Error         | napaka / odstopanje    | Mismatch: `intention` ≠ `actuallyOn = (ldrValue > LDR_THRESHOLD)` |

The loop closes because the controller's input depends on both the command *and* the sensor — the system does not trust the actuator, it trusts the sensor.

### Failure modes

| State            | Meaning                                                      | Response                                                        |
|------------------|--------------------------------------------------------------|-----------------------------------------------------------------|
| `ok`             | intention and sensor agree                                   | All indicators off, banner green                                |
| `napaka_vklop`   | intention=ON but LDR dark (LED removed / burned / covered)   | Red reserve on, blue blinks, banner red                         |
| `napaka_izklop`  | intention=OFF but LDR bright (external light leakage)        | Blue blinks only, banner red — no off-substitute is possible    |

### Wire format

Three message types over WebSocket (port 8888 to browsers, 8811 to ESP32 — Node bridges them):

```json
// browser → ESP32 (button click)
{ "tipSporočila": "LED",         "pin": 5,  "vrednost": 1 }

// ESP32 → browser (continuous, ~20 Hz)
{ "tipSporočila": "fotoupornik", "pin": 32, "vrednost": 612 }

// ESP32 → browser (on state change only)
{ "tipSporočila": "status",      "stanje": "ok" }
```

`vrednost` for LED is `1` (on) or `0` (off). `vrednost` for LDR is the raw 12-bit ADC value (0–4095). `stanje` is one of `ok`, `napaka_vklop`, `napaka_izklop`.

### Tunable parameters

All in `src/main.cpp`:

- **`LDR_THRESHOLD`** (default 500) — the boundary between "LED working" and "LED not working". Watch the live chart: note the LDR plateau when the LED is on and uncovered (≈600–700 with the demo LED), and when the LED is off or covered (≈100–400). Set the threshold roughly halfway between them. Re-tune if ambient light changes drastically.
- **`COMMAND_GRACE_MS`** (default 150) — how long the comparator stays silent after the intention flips. Stops the comparator from firing one spurious error every button press while the LED and ADC settle.
- **`BLINK_INTERVAL_MS`** (default 250) — half-period of the alarm blink (4 Hz). Non-blocking, based on `millis()`.

### Running

**1. Node hub.** From `homework-04/node-homework-04/`:

```powershell
npm install     # first time only
npm start
```

**2. ESP32.** From `homework-04/`:

```powershell
pio run --target upload
pio device monitor          # optional, 115200 baud
```

**3. Browser.** Open `http://<laptop-ip>/`. You should see:

- ON/OFF buttons (`Prižgi` / `Ugasni`).
- A status banner (green when OK, red on error).
- A "Fotoupornik" readout with the live ADC value.
- A canvas line-graph of the LDR over the last ~30 s.
- Raw JSON of the last `fotoupornik` message.

Try this sequence: press `Prižgi` → banner stays green, chart spikes up. Cover the LDR with a piece of paper (simulating a burned LED): chart drops, banner turns red with `napaka_vklop`, red reserve LED lights, blue blinks. Uncover: everything recovers within ~150 ms.

### Status caching

Node caches the most recent `status` message and replays it to each new browser connection. Without this, a browser opened *after* the chip last reported its state would see "Čakam na stanje …" until something changes — the chip emits status on **change** only, never as a heartbeat.

### Troubleshooting

- **Banner stuck on "Čakam na stanje …"** → no `status` has ever flowed since Node started. Check the chip is connected (look for `Brskalnik se je povezal` and `ESP32 se je povezal` in the Node terminal). If the chip is connected but no `Status: ok` line ever appears, your initial state already matches reality (intention=OFF + LDR dark) — flip the LED on once to force a state change.
- **Comparator says `napaka_vklop` even when the LED is clearly on** → threshold too high. Watch the chart, find the LDR plateau when lit, set `LDR_THRESHOLD` below it.
- **System flaps between OK and error on each button press** → `COMMAND_GRACE_MS` is too short, or the LDR is somehow slower than expected. Raise it to 300 ms.
- **Status banner doesn't update after Node code change** → Node only re-reads JS on restart. `Ctrl+C` and `npm start` again. (HTML changes don't require a restart — Express re-reads the file per request.)
- **`EACCES: permission denied 0.0.0.0:80`** → port 80 needs admin on Windows. Run PowerShell as admin or change `HTTP_PORT` in `node-homework-04.js` to 8080.

### Going further

- **Hysteresis.** Replace the single threshold with two: cross above 550 to count as "bright", drop below 300 to count as "dark". Reduces flapping near the boundary.
- **Auto-calibration.** On boot, with the LED commanded off, sample ambient LDR for 1 s; set the threshold to `ambient + margin`. Adapts to room light at startup but not afterwards.
- **PWM dimming.** Make the setpoint analog (0–255) instead of binary, drive the LED with `ledcWrite`, and have the comparator check that the LDR tracks the requested level. That's the next step from binary on/off to a proper continuous-error control loop.

---

## Slovensko

### Strojna oprema

- Razvojna ploščica ESP32 na USB.
- **Zelena LED** na **GPIO 5** — krmiljen izvršni element. Prek omejevalnega upora (≈220 Ω) na GND.
- **Rdeča LED** na **GPIO 21** — rezervni izvršni element; zasveti, ko komparator zazna `napaka_vklop` (ukazana VKLOP, a senzor zaznava temo). Enako ožičenje kot zelena.
- **Vgrajena modra LED** na **GPIO 2** — alarm; utripa, ko je aktivno katerokoli stanje napake. Brez zunanjega ožičenja.
- **Fotoupornik (LDR)** na **GPIO 32** v napetostnem delilniku:
  `3.3 V → LDR → GPIO 32 → 10 kΩ → GND`.
  Veliko svetlobe = visoka ADC vrednost, tema = nizka ADC vrednost. LDR postavi tako, da neposredno vidi zeleno LED (dotik je v redu).
- Prenosnik na istem WiFi-ju kot čip, ki poganja Node.

### Razporeditev map

```
homework-04/
├── platformio.ini             ← nastavitve ESP32 (WebSockets + ArduinoJson)
├── src/main.cpp               ← WebSocket odjemalec, komparator, logika napak
├── node-homework-04/          ← Node stran
│   ├── node-homework-04.js      ← Express + dva WSS + predpomnilnik stanja
│   ├── node-homework-04.html    ← brskalniški vmesnik (gumbi + graf + pas)
│   └── package.json
└── README.md
```

### Zgradba kontrolnega sistema

```
    ukaz                                                              
 (setpoint)      napaka       krmilna logika                          
     │             │              │                                   
     ▼             ▼              ▼                                   
 ┌───────┐    ┌───────┐      ┌──────────┐    ┌──────────────┐
 │   UI  │───►│ PRI-  │─────►│ KRMILNIK │───►│   IZVRŠNI    │──┐
 │ gumbi │    │MERJAL.│      │ (logika) │    │ ELEMENT zel. │  │
 └───────┘    └───────┘      └──────────┘    └──────────────┘  │
                  ▲                                              │
                  │                              svetloba ───────┘
                  │                                ▼
                  │                          ┌──────────┐
                  └─── povratna zveza ───────│  SENZOR  │
                          (LDR vrednost)     │ LDR + ADC│
                                             └──────────┘
```

Preslikava v jezik kontrolnih sistemov:

| Vloga                | Angleško            | V tem projektu                                                       |
|----------------------|---------------------|----------------------------------------------------------------------|
| Nastavitvena vred.   | setpoint            | `greenIntention` (true/false), nastavi klik gumba                    |
| Primerjalnik         | comparator          | `if/else` v `loop()`, ki razvrsti ok / napaka_vklop / napaka_izklop  |
| Krmilnik             | controller          | Vrstice za komparatorjem, ki krmilijo rdečo/modro in pošljejo `status`|
| Izvršni element      | actuator            | Zelena LED (in rdeča LED kot rezerva ob odpovedi)                    |
| Proces               | process             | Širjenje svetlobe od LED do površine LDR                              |
| Senzor               | sensor              | LDR + napetostni delilnik + 12-bitni ADC na GPIO 32                   |
| Napaka               | error               | Neujemanje: `ukaz` ≠ `actuallyOn = (ldrValue > LDR_THRESHOLD)`        |

Zanka je sklenjena, ker vhod krmilnika hkrati odvisi od ukaza **in** senzorja — sistem ne zaupa izvršnemu elementu, zaupa senzorju.

### Načini odpovedi

| Stanje           | Pomen                                                          | Odziv                                                          |
|------------------|----------------------------------------------------------------|-----------------------------------------------------------------|
| `ok`             | ukaz in senzor se ujemata                                      | Vsi indikatorji ugasnjeni, pas zelen                            |
| `napaka_vklop`   | ukaz=VKLOP, a LDR temno (LED odstranjena/pregorela/zakrita)    | Rdeča rezerva sveti, modra utripa, pas rdeč                     |
| `napaka_izklop`  | ukaz=IZKLOP, a LDR svetlo (zunanja svetloba)                   | Samo modra utripa, pas rdeč — nadomestek za "izklop" ni mogoč   |

### Oblika sporočil

Tri vrste sporočil prek WebSocket-ov (vrata 8888 za brskalnik, 8811 za ESP32 — Node ju povezuje):

```json
// brskalnik → ESP32 (klik gumba)
{ "tipSporočila": "LED",         "pin": 5,  "vrednost": 1 }

// ESP32 → brskalnik (zvezno, ~20 Hz)
{ "tipSporočila": "fotoupornik", "pin": 32, "vrednost": 612 }

// ESP32 → brskalnik (samo ob spremembi stanja)
{ "tipSporočila": "status",      "stanje": "ok" }
```

`vrednost` pri LED je `1` (vklop) ali `0` (izklop). `vrednost` pri LDR je surova 12-bitna vrednost ADC-ja (0–4095). `stanje` je `ok`, `napaka_vklop` ali `napaka_izklop`.

### Nastavljive vrednosti

Vse v `src/main.cpp`:

- **`LDR_THRESHOLD`** (privzeto 500) — meja med "LED dela" in "LED ne dela". Opazuj živi graf: zabeleži vrednost LDR, ko je LED prižgana in odkrita (≈600–700 s preizkušeno LED diodo), in ko je LED ugasnjena ali zakrita (≈100–400). Prag postavi približno na sredo med njima. Pri večji spremembi okolne svetlobe ponovno umeri.
- **`COMMAND_GRACE_MS`** (privzeto 150) — koliko časa komparator molči po spremembi ukaza. Preprečuje, da bi komparator ob vsakem kliku sprožil eno lažno napako, dokler se LED in ADC ne ustalita.
- **`BLINK_INTERVAL_MS`** (privzeto 250) — polovica periode alarmnega utripanja (4 Hz). Neblokirajoče, osnovano na `millis()`.

### Zagon

**1. Node zvezdišče.** V mapi `homework-04/node-homework-04/`:

```powershell
npm install     # samo prvič
npm start
```

**2. ESP32.** Iz mape `homework-04/`:

```powershell
pio run --target upload
pio device monitor          # neobvezno, 115200 baud
```

**3. Brskalnik.** Odpri `http://<IP-prenosnika>/`. Videti moraš:

- Gumba `Prižgi` in `Ugasni`.
- Pas stanja (zelen, ko je OK, rdeč ob napaki).
- Izpis "Fotoupornik" s tekočo ADC vrednostjo.
- Risbo na platnu (zadnjih ~30 s).
- Surov JSON zadnjega sporočila `fotoupornik`.

Preizkusni potek: klikni `Prižgi` → pas ostane zelen, graf skoči navzgor. LDR pokrij s košček papirja (simulacija pregorele LED): graf pade, pas postane rdeč z `napaka_vklop`, rdeča rezerva zasveti, modra utripa. Odkrij: vse se v ~150 ms povrne.

### Predpomnjenje stanja

Node hrani zadnje sporočilo `status` in ga ob vsaki novi povezavi brskalnika ponovno pošlje. Brez tega bi brskalnik, odprt **po** tem, ko je čip zadnjič javil svoje stanje, obtičal pri "Čakam na stanje …" — čip pošlje status le ob **spremembi** stanja, ne kot redni signal.

### Odpravljanje težav

- **Pas obtiči na "Čakam na stanje …"** → po zagonu Node še ni preteklo nobeno `status` sporočilo. Preveri, ali je čip povezan (v Node terminalu morata biti vidni vrstici `Brskalnik se je povezal` in `ESP32 se je povezal`). Če je čip povezan, a vrstica `Status: ok` ni nikoli izpisana, se začetno stanje že ujema z realnostjo (ukaz=IZKLOP + LDR temno) — enkrat prižgi LED, da sprožiš spremembo stanja.
- **Komparator javi `napaka_vklop`, čeprav je LED očitno prižgana** → prag je previsok. Opazuj graf, najdi plato LDR-ja pri prižgani LED, postavi `LDR_THRESHOLD` pod njega.
- **Sistem ob vsakem kliku gumba zaniha med OK in napako** → `COMMAND_GRACE_MS` je prekratek, ali pa je LDR počasnejši od pričakovanega. Povečaj na 300 ms.
- **Pas se po spremembi Node kode ne posodobi** → Node ponovno prebere JS le ob ponovnem zagonu. `Ctrl+C` in `npm start`. (Pri HTML zagon ni potreben — Express ob vsaki zahtevi prebere datoteko.)
- **`EACCES: permission denied 0.0.0.0:80`** → vrata 80 na Windowsih potrebujejo skrbniške pravice. Zaženi PowerShell kot skrbnik ali spremeni `HTTP_PORT` v `node-homework-04.js` na 8080.

### Korak naprej

- **Histereza.** Eno mejo zamenjaj z dvema: presežek čez 550 šteje za "svetlo", padec pod 300 za "temno". Zmanjša nihanje blizu meje.
- **Samoumerjanje.** Ob zagonu, z ukazom za izklop LED, 1 s vzorči ambient LDR; prag postavi na `ambient + rezerva`. Prilagodi se sobni svetlobi ob zagonu, kasneje pa ne.
- **PWM zatemnjevanje.** Naredi nastavitveno vrednost analogno (0–255) namesto binarno, krmili LED z `ledcWrite` in komparator naj preveri, ali LDR sledi zahtevani ravni. To je naslednji korak od binarnega vklop/izklop h klasični zvezni regulacijski zanki.
