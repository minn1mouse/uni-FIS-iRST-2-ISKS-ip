# Example 05b — Control the LED over HTTP (IoT "hello world")

Builds on [example-05a](../example-05a/README.md). The `/1` and `/0` routes no longer just return different pages — they now **change the state of the onboard LED**. Visit `/1` and the LED turns on; visit `/0` and it turns off. This is the canonical IoT pattern: an actuator driven by HTTP.

Nadgradnja [example-05a](../example-05a/README.md). Poti `/1` in `/0` ne vračata več le različnih strani — zdaj **spreminjata stanje vgrajene LED diode**. Odpri `/1` in LED se prižge; odpri `/0` in se ugasne. To je tipični IoT vzorec: aktuator, krmiljen prek HTTP-ja.

---

## English

### Prerequisites

Same hardware and `secrets.h` setup as the previous examples ([example-02](../example-02/README.md#one-time-configuration)).

### What's new in this example

- **Shared state.** Two globals carry the LED configuration across handlers and the main loop:
  ```cpp
  uint8_t LED1pin = 2;
  bool    LED1status = LOW;
  ```
- **Handlers with side effects.** `handle_1` and `handle_0` update `LED1status` *before* sending the HTML response. The HTTP handler stays fast (just sets a flag) — the GPIO write happens in `loop()`.
- **`loop()` drives the pin from state.** Every iteration reads `LED1status` and writes the corresponding HIGH/LOW to `LED1pin`. The handlers and the hardware update are decoupled.
- The HTML on `/` is now an instruction page: "add `/1` to turn on, `/0` to turn off."

### Build, upload, monitor

```bash
pio run
pio run --target upload
pio device monitor
```

### Expected behaviour

After the WiFi log + IP printout, in a browser on the same WiFi:

| URL | Response | Side effect |
| --- | --- | --- |
| `http://<esp32-ip>/`  | instruction page | none |
| `http://<esp32-ip>/1` | "Prejet ukaz za vklop LED diode." | onboard LED turns **on** |
| `http://<esp32-ip>/0` | "Prejet ukaz za izklop LED diode." | onboard LED turns **off** |

The LED stays in the last commanded state until the next `/1` or `/0` request.

### Going further

Replace the onboard LED with a **relay module** on the same pin and you can switch a real device — a lamp, a fan, a kettle — over the same HTTP API. Powered from a battery, the ESP32 becomes a fully standalone networked actuator.

---

## Slovensko

### Predpogoji

Enaka strojna oprema in nastavitev `secrets.h` kot v prejšnjih primerih ([example-02](../example-02/README.md#enkratna-nastavitev)).

### Kaj je novega v tem primeru

- **Skupno stanje.** Dve globalni spremenljivki nosita konfiguracijo LED diode med funkcijami in glavno zanko:
  ```cpp
  uint8_t LED1pin = 2;
  bool    LED1status = LOW;
  ```
- **Funkcije s stranskimi učinki.** `handle_1` in `handle_0` *pred* pošiljanjem HTML odgovora posodobita `LED1status`. Funkcija HTTP ostane hitra (le nastavi zastavico) — zapis na GPIO se zgodi v `loop()`.
- **`loop()` krmili pin glede na stanje.** Vsaka ponovitev prebere `LED1status` in zapiše ustrezni HIGH/LOW na `LED1pin`. Funkcije za zahteve in posodobitev strojne opreme so ločene.
- HTML na `/` je zdaj navodilna stran: "dodaj `/1` za vklop, `/0` za izklop."

### Prevajanje, nalaganje, spremljanje

```bash
pio run
pio run --target upload
pio device monitor
```

### Pričakovano obnašanje

Po WiFi izpisu in IP-ju v brskalniku (na istem WiFi-ju):

| URL | Odgovor | Stranski učinek |
| --- | --- | --- |
| `http://<esp32-ip>/`  | navodilna stran | brez |
| `http://<esp32-ip>/1` | "Prejet ukaz za vklop LED diode." | vgrajena LED se **prižge** |
| `http://<esp32-ip>/0` | "Prejet ukaz za izklop LED diode." | vgrajena LED se **ugasne** |

LED ostane v zadnjem ukazanem stanju do naslednje zahteve `/1` ali `/0`.

### Korak naprej

Zamenjaj vgrajeno LED z **relejskim modulom** na isti nožici in lahko prek istega HTTP API-ja vklopiš pravo napravo — luč, ventilator, kuhalnik. Z napajanjem iz baterije postane ESP32 popolnoma samostojni omrežni aktuator.
