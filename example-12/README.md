# Example 12 — Read a potentiometer (ADC on GPIO 34)

First analogue input. The ESP32 reads a 10 kΩ potentiometer wired to **GPIO 34** with `analogRead()` and prints the value to the serial monitor every 50 ms. No WiFi, no HTTP, no LED control — purely the chip's ADC pipeline.

Prva analogna meritev. ESP32 z `analogRead()` vsakih 50 ms prebere 10 kΩ potenciometer, priključen na **GPIO 34**, in vrednost izpiše v serijski monitor. Brez WiFi-ja, brez HTTP-ja, brez krmiljenja LED — samo ADC veriga čipa.

---

## English

### Hardware

- ESP32 dev board on USB.
- 10 kΩ potentiometer:
  - Outer pins to **3.3 V** and **GND**.
  - Wiper (middle pin) to **GPIO 34**.

GPIO 34 is on **ADC1** and is input-only — it doesn't have an internal pull-up/pull-down, which is fine here because the potentiometer always drives the line to a defined voltage.

### What's new in this example

- **`analogRead(pin)`** — the ESP32's 12-bit ADC. Returns an integer in `0..4095`, mapping the input voltage `0..3.3 V` linearly. With the wiper at one end you get `0`, at the other end `~4095`, and in between values follow the dial.
- **`Serial.println()` as a probe** — there's no HTML, no WebSocket; the serial monitor *is* the output device. Twist the knob and watch the numbers move. Many later examples reuse this exact reading on GPIO 34 (see [joined-example-04](../joined-example-04/) and [joined-example-04a](../joined-example-04a/) for the streamed-to-browser versions).
- **The two-blink boot indicator** at the start of `setup()` — drives GPIO 2 HIGH/LOW twice. Useful to see at a glance whether the chip rebooted (e.g. after a brown-out).

### Build, upload, monitor

From inside `example-12/`:

```powershell
pio run                       # build
pio run --target upload       # flash
pio device monitor            # 115200 baud
```

No WiFi credentials needed — `secrets.h` is not used here.

### Expected serial output

```
0
0
12
347
1024
2048
3500
4095
4095
```

Twist the knob → values change smoothly. Hold steady → values are stable (small ±1 jitter is normal ADC noise).

### Troubleshooting

- **Always reads `0`** → the wiper is sitting at 0 V. Most likely cause: one of the outer pins isn't connected (typically the 3.3 V leg). Or you didn't twist the knob.
- **Always reads `4095`** → wiper sitting at 3.3 V. Twist the knob.
- **Values jump around wildly even without touching the pot** → the wire to GPIO 34 is loose, or you wired the wiper to a different pin and GPIO 34 is floating. Re-check the middle leg.
- **No output at all in the monitor** → wrong baud (must be `115200`), or the wrong project was uploaded (the active PlatformIO project in VSCode's bottom status bar isn't necessarily the folder of the file you have open).

---

## Slovensko

### Strojna oprema

- ESP32 razvojna ploščica na USB.
- 10 kΩ potenciometer:
  - Zunanji nožici na **3.3 V** in **GND**.
  - Drsnik (srednji pin) na **GPIO 34**.

GPIO 34 je na **ADC1** in je samo vhod — nima notranjega potegnjenega upora, kar je tu v redu, ker potenciometer vedno žene linijo na določeno napetost.

### Kaj je novega v tem primeru

- **`analogRead(pin)`** — 12-bitni ADC ESP32. Vrne celo število `0..4095`, ki linearno preslika vhodno napetost `0..3.3 V`. Drsnik na enem koncu da `0`, na drugem `~4095`, vmes vrednosti sledijo gumbu.
- **`Serial.println()` kot opazovalec** — nimaš HTML-ja in nobenega WebSocket-a; serijski monitor *je* izhodna naprava. Zavrti gumb in opazuj številke. Marsikateri kasnejši primer uporabi prav to branje na GPIO 34 (glej [joined-example-04](../joined-example-04/) in [joined-example-04a](../joined-example-04a/) za različici s pretokom v brskalnik).
- **Dvojno utripanje ob zagonu** na začetku `setup()` — žene GPIO 2 dvakrat HIGH/LOW. Uporabno, da na hitro vidiš, ali se je čip ponovno zagnal (npr. po brown-outu).

### Prevajanje, nalaganje, spremljanje

V mapi `example-12/`:

```powershell
pio run                       # prevedi
pio run --target upload       # naloži kodo
pio device monitor            # 115200 baud
```

WiFi podatki niso potrebni — `secrets.h` se tu ne uporablja.

### Pričakovani serijski izpis

```
0
0
12
347
1024
2048
3500
4095
4095
```

Zavrti gumb → vrednosti se gladko spreminjajo. Drži pri miru → vrednosti so stabilne (rahel ±1 nihaj je običajen ADC šum).

### Odpravljanje težav

- **Vedno izpiše `0`** → drsnik je na 0 V. Najpogostejši vzrok: ena od zunanjih nožic ni priključena (običajno noga 3.3 V). Ali pa nisi zavrtela gumba.
- **Vedno izpiše `4095`** → drsnik je na 3.3 V. Zavrti gumb.
- **Vrednosti skačejo, čeprav se gumba ne dotikaš** → žica do GPIO 34 je odklopljena ali pa si drsnik priklopila na drug pin in GPIO 34 lebdi. Preveri srednjo nožico.
- **V monitorju ni nobenega izpisa** → napačen baud (mora biti `115200`) ali pa je bil naložen napačen projekt (aktivni PlatformIO projekt v spodnji statusni vrstici VSCode ni nujno mapa odprte datoteke).
