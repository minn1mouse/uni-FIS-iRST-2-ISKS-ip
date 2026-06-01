# Example 04 — HTTP server serving HTML with UTF-8

Builds on [example-03](../example-03/README.md). The HTTP server now returns a full **HTML page** (not just plain text), stored in flash via `PROGMEM` and a C++11 **raw string literal**, and demonstrates that Slovene characters (čšž) render correctly with `charset="UTF-8"` and `lang="sl"`.

Nadgradnja [example-03](../example-03/README.md). HTTP strežnik zdaj vrne celotno **HTML stran** (ne le navadnega besedila), shranjeno v flash pomnilniku prek `PROGMEM` in **surovega niza** (C++11 raw string literal). Demonstrira, da se slovenske črke (čšž) v brskalniku pravilno prikažejo zaradi `charset="UTF-8"` in `lang="sl"`.

---

## English

### Prerequisites

Same hardware and WiFi setup as [example-02](../example-02/README.md#one-time-configuration) and [example-03](../example-03/README.md). The build also needs the shared `secrets.h` configured.

### What's new in this example

- **`const char HTML[] PROGMEM = R"rawliteral( ... )rawliteral";`** — the full HTML document lives as a constant in flash memory (`PROGMEM`) so it doesn't consume RAM. The `R"rawliteral( ... )rawliteral"` is a C++11 raw string literal that lets you embed multi-line HTML without escaping quotes or backslashes.
- **`server.send(200, "text/html; charset=UTF-8", HTML)`** — the response body is the HTML constant rather than an inline string. The explicit UTF-8 content-type, together with `<meta charset="UTF-8">` and `<html lang="sl">` in the document, ensures Slovene letters (`č`, `š`, `ž`) render correctly across browsers.

### Build, upload, monitor

```bash
pio run                    # compile
pio run --target upload    # flash to the connected ESP32
pio device monitor         # open serial monitor (115200 baud)
```

### Expected behaviour

Serial monitor shows the usual WiFi + IP printout. Open `http://<esp32-ip>/` in a browser on the same WiFi — the page renders:

> # Pozdravljen svet iz esp32, čšž-ji delujejo!

If the Slovene characters appear as mojibake (e.g. `Ä?` boxes), one of the charset declarations is missing — keep all three in place: HTTP header, `<meta charset>`, `<html lang>`.

---

## Slovensko

### Predpogoji

Enaka strojna oprema in nastavitev WiFi-ja kot v [example-02](../example-02/README.md#enkratna-nastavitev) in [example-03](../example-03/README.md). Gradnja potrebuje tudi nastavljen skupni `secrets.h`.

### Kaj je novega v tem primeru

- **`const char HTML[] PROGMEM = R"rawliteral( ... )rawliteral";`** — celoten HTML dokument je shranjen kot konstanta v flash pomnilniku (`PROGMEM`), zato ne porablja RAM-a. Zapis `R"rawliteral( ... )rawliteral"` je C++11 surovi niz, ki dovoljuje pisanje večvrstičnega HTML-ja brez ubežnih znakov za narekovaje ali povratne poševnice.
- **`server.send(200, "text/html; charset=UTF-8", HTML)`** — vsebina odgovora je konstanta HTML namesto vrstičnega niza. Izrecno določen UTF-8 tip vsebine skupaj z `<meta charset="UTF-8">` in `<html lang="sl">` v dokumentu poskrbi, da se slovenske črke (`č`, `š`, `ž`) pravilno prikažejo v vseh brskalnikih.

### Prevajanje, nalaganje, spremljanje

```bash
pio run                    # prevedi
pio run --target upload    # naloži na priključen ESP32
pio device monitor         # odpri serijski monitor (115200 baud)
```

### Pričakovano obnašanje

Serijski monitor pokaže običajen WiFi izpis in IP. Odpri `http://<esp32-ip>/` v brskalniku na istem WiFi-ju — stran prikaže:

> # Pozdravljen svet iz esp32, čšž-ji delujejo!

Če se slovenske črke prikažejo kot napačni znaki (npr. `Ä?` kvadratki), manjka ena od deklaracij kodne strani — ohrani vse tri: HTTP glavo, `<meta charset>` in `<html lang>`.
