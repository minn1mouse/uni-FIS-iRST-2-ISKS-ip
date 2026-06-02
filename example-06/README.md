# Example 06 — Styled HTML pages from LittleFS

Same behaviour as [example-05c](../example-05c/README.md) — `/1` turns the onboard LED on, `/0` turns it off — but the three pages served from `data/` are now **styled with embedded CSS**. The buttons are visible blue blocks instead of plain `<a>` links, and the page uses a centred Helvetica layout. The C++ code is unchanged from example-05c; the lesson lives in the `.html` files in `data/`.

Enako vedenje kot v [example-05c](../example-05c/README.md) — `/1` prižge vgrajeno LED, `/0` jo ugasne — vendar so tri strani, postrežene iz `data/`, zdaj **slogovno oblikovane z vgrajenim CSS-jem**. Gumbi so vidni modri bloki namesto navadnih `<a>` povezav, postavitev strani pa je sredinska s pisavo Helvetica. C++ koda je enaka kot v example-05c; lekcija živi v datotekah `.html` v mapi `data/`.

---

## English

### Prerequisites

Same hardware and `secrets.h` setup as the earlier networked examples ([example-02](../example-02/README.md#one-time-configuration)).

### What's new in this example

- **`<style>` blocks inside each `.html` file.** The CSS is embedded directly in `<head>` — no separate `.css` file (yet). Three nearly-identical pages share the same styles: only the final `<p>` differs.
- **Visible button styling.** The two `<a>` elements are rendered as full-width blue blocks (`.gumb` class) instead of underlined links. Click area matches the visual.
- **Centred page layout.** `text-align: center` on `<html>`, generous `margin-top` on `<body>`, and centred max-width buttons make the page look intentional rather than the browser default.

The C++ in `src/main.cpp` is unchanged from example-05c — same `LittleFS.begin()`, same `sendFile()` helper, same three routes. All the work happens in the HTML/CSS.

### Build, upload, monitor

Run from inside `example-06/`. As with example-05c, **two upload steps**:

```bash
pio run --target upload       # flash the code
pio run --target uploadfs     # flash the data/ folder
pio device monitor            # 115200 baud
```

### Expected behaviour

After the WiFi connect log + IP, open `http://<esp32-ip>/` in a browser on the same WiFi. You see a centred page with title "Esp32 spletni strežnik", subtitle "Priklop na WiFi usmerjevalnik", two blue button-blocks (**Prižgi LED** / **Ugasni LED**), and the status paragraph. Clicking a button hits `/1` or `/0`, the LED state changes, and the page reloads with the matching status text.

### Going further

The three HTML files duplicate the entire `<style>` block. A natural next step is to extract the CSS into a single `data/style.css` and link it with `<link rel="stylesheet" href="/style.css">` in each page — then update `main.cpp` to also serve `.css` files (one `server.on("/style.css", ...)` line, or use `server.serveStatic("/", LittleFS, "/")` to serve the whole folder generically).

---

## Slovensko

### Predpogoji

Enaka strojna oprema in nastavitev `secrets.h` kot v prejšnjih omrežnih primerih ([example-02](../example-02/README.md#enkratna-nastavitev)).

### Kaj je novega v tem primeru

- **`<style>` bloki znotraj vsake `.html` datoteke.** CSS je vgrajen neposredno v `<head>` — brez ločene `.css` datoteke (zaenkrat). Tri skoraj enake strani si delijo iste sloge: razlikuje se le zadnji `<p>`.
- **Vidno oblikovanje gumbov.** Dva `<a>` elementa sta prikazana kot polna modra bloka (razred `.gumb`), ne kot podčrtane povezave. Območje klika ustreza vizualnemu izgledu.
- **Sredinska postavitev strani.** `text-align: center` na `<html>`, velikodušni `margin-top` na `<body>` in sredinsko poravnani gumbi z največjo širino dajo strani namensko videz namesto privzetega brskalniškega.

C++ v `src/main.cpp` je nespremenjen v primerjavi z example-05c — isti `LittleFS.begin()`, ista pomožna funkcija `sendFile()`, iste tri poti. Vsa novost je v HTML/CSS.

### Prevajanje, nalaganje, spremljanje

Iz mape `example-06/`. Kot pri example-05c, **dva koraka nalaganja**:

```bash
pio run --target upload       # naloži kodo
pio run --target uploadfs     # naloži mapo data/
pio device monitor            # 115200 baud
```

### Pričakovano obnašanje

Po WiFi izpisu in IP-ju v brskalniku (na istem WiFi-ju) odpri `http://<esp32-ip>/`. Vidiš sredinsko postavljeno stran z naslovom "Esp32 spletni strežnik", podnaslovom "Priklop na WiFi usmerjevalnik", dvema modrima gumbnima blokoma (**Prižgi LED** / **Ugasni LED**) in odstavkom s statusom. Klik na gumb sproži `/1` ali `/0`, stanje LED se spremeni, stran pa se osveži z ustreznim statusnim besedilom.

### Korak naprej

Tri HTML datoteke podvajajo cel `<style>` blok. Naraven naslednji korak je izvleči CSS v eno samo datoteko `data/style.css` in jo povezati z `<link rel="stylesheet" href="/style.css">` v vsaki strani — nato pa v `main.cpp` dodati strežbo `.css` datotek (ena vrstica `server.on("/style.css", ...)` ali pa `server.serveStatic("/", LittleFS, "/")` za splošno strežbo cele mape).
