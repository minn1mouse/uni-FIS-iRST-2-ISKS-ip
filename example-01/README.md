# Example 01 — Blink (onboard LED on GPIO 2)

The first example in the course. It blinks the **onboard LED on GPIO 2** of an ESP32 dev board at roughly **1 Hz** (500 ms on, 500 ms off). The sketch serves both as a "hello world" for the toolchain (PlatformIO, ESP32 framework, USB driver, upload flow) and as the canonical introduction to `pinMode()`, `digitalWrite()`, and `delay()`.

Prvi primer pri predmetu. Utripa **vgrajeno LED diodo na nožici GPIO 2** razvojne ploščice ESP32 s približno **1 Hz** (500 ms prižgana, 500 ms ugasnjena). Skica služi kot "hello world" za orodjarno (PlatformIO, ogrodje ESP32, USB gonilnik, postopek nalaganja) in kot osnovni uvod v funkcije `pinMode()`, `digitalWrite()` in `delay()`.

---

## English

### Hardware

- ESP32 development board (configured as `esp32dev` in `platformio.ini`)
- USB cable for power and flashing
- Onboard LED on **GPIO 2** (blue, on most ESP32 dev boards)

No external wiring is required.

### How it works

- `setup()` runs once and sets GPIO 2 as an output (`pinMode(2, OUTPUT)`).
- `loop()` runs repeatedly: drives the pin HIGH (LED on) → `delay(500)` → drives it LOW (LED off) → `delay(500)`. The result is a steady ~1 Hz blink.

### Build, upload, monitor

Run all commands from inside `example-01/`:

```bash
pio run                    # compile
pio run --target upload    # flash to the connected ESP32
pio device monitor         # open serial monitor (this sketch prints nothing)
pio run --target clean     # remove build artifacts
```

### Project layout

- `src/main.cpp` — the blink sketch with bilingual inline comments.
- `include/` — project-wide headers (empty for this example).
- `lib/` — project-private libraries (empty for this example).
- `test/` — PlatformIO Test Runner tests (empty for this example).
- `platformio.ini` — board, framework, and dependency configuration.

### Expected result

After upload the onboard blue LED on GPIO 2 blinks at ~1 Hz. If your dev board doesn't have an LED tied to GPIO 2, nothing visible happens — change the pin in `main.cpp` to whichever GPIO drives your board's onboard LED.

---

## Slovensko

### Strojna oprema

- Razvojna ploščica ESP32 (nastavljena kot `esp32dev` v `platformio.ini`)
- USB kabel za napajanje in nalaganje
- Vgrajena LED dioda na **GPIO 2** (modra, na večini ESP32 razvojnih ploščic)

Dodatno ožičenje ni potrebno.

### Kako deluje

- `setup()` se izvede enkrat in nastavi GPIO 2 kot izhod (`pinMode(2, OUTPUT)`).
- `loop()` se izvaja ponavljajoče: nožico postavi na HIGH (LED prižgana) → `delay(500)` → postavi na LOW (LED ugasnjena) → `delay(500)`. Rezultat je enakomerno utripanje s frekvenco ~1 Hz.

### Prevajanje, nalaganje, spremljanje

Vse ukaze poganjajte iz mape `example-01/`:

```bash
pio run                    # prevedi
pio run --target upload    # naloži na priključen ESP32
pio device monitor         # odpri serijski monitor (skica nič ne izpisuje)
pio run --target clean     # počisti zgrajene datoteke
```

### Zgradba projekta

- `src/main.cpp` — skica za utripanje z dvojezičnimi komentarji.
- `include/` — skupne glave projekta (v tem primeru prazno).
- `lib/` — projektno-zasebne knjižnice (v tem primeru prazno).
- `test/` — testi za PlatformIO Test Runner (v tem primeru prazno).
- `platformio.ini` — nastavitve ploščice, ogrodja in odvisnosti.

### Pričakovani rezultat

Po nalaganju modra vgrajena LED dioda na GPIO 2 utripa s frekvenco ~1 Hz. Če vaša ploščica nima LED diode na GPIO 2, vidnega utripanja ne bo — v `main.cpp` zamenjajte številko nožice s tisto, ki krmili vgrajeno LED diodo vaše ploščice.
