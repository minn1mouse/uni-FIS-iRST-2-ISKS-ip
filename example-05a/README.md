# Example 05a — Multiple HTTP routes

Builds on [example-04](../example-04/README.md). The HTTP server now exposes **three different paths**, each mapped to its own handler returning a different HTML page. Visiting `/`, `/1`, or `/0` gives a different response.

Nadgradnja [example-04](../example-04/README.md). HTTP strežnik zdaj ponuja **tri različne poti**, vsaka preslikana na svojo funkcijo, ki vrne drugačno HTML stran. Odpri `/`, `/1` ali `/0` za različne odgovore.

---

## English

### Prerequisites

Same hardware and `secrets.h` setup as previous examples ([example-02](../example-02/README.md#one-time-configuration)).

### What's new in this example

- Three `const char HTML*[] PROGMEM` constants instead of one — each holds the body for a different page.
- Three route handlers (`handle_root`, `handle_1`, `handle_0`) — each just sends its own HTML constant.
- Three `server.on(path, handler)` registrations in `setup()`:
  - `server.on("/",  handle_root)` — default greeting
  - `server.on("/1", handle_1)` — page of ones
  - `server.on("/0", handle_0)` — page of zeros
- Anything else (e.g. `/foo`) returns a 404 automatically — the `WebServer` library handles that for you with no extra code.

### Build, upload, monitor

```bash
pio run
pio run --target upload
pio device monitor
```

### Expected behaviour

After the usual WiFi log + IP printout, open in a browser on the same WiFi:

| URL | Response |
| --- | --- |
| `http://<esp32-ip>/`  | **Pozdravljen svet iz esp32, čšž-ji delujejo!** |
| `http://<esp32-ip>/1` | **Spletna stran enojk 111111111111 !** |
| `http://<esp32-ip>/0` | **Spletna stran ničel 000000000000 !** |
| `http://<esp32-ip>/anything-else` | 404 Not Found |

---

## Slovensko

### Predpogoji

Enaka strojna oprema in nastavitev `secrets.h` kot v prejšnjih primerih ([example-02](../example-02/README.md#enkratna-nastavitev)).

### Kaj je novega v tem primeru

- Tri konstante `const char HTML*[] PROGMEM` namesto ene — vsaka hrani vsebino svoje strani.
- Tri funkcije za obravnavo zahteve (`handle_root`, `handle_1`, `handle_0`) — vsaka pošlje svojo HTML konstanto.
- Tri klice `server.on(pot, funkcija)` v `setup()`:
  - `server.on("/",  handle_root)` — privzeti pozdrav
  - `server.on("/1", handle_1)` — stran enojk
  - `server.on("/0", handle_0)` — stran ničel
- Karkoli drugega (npr. `/foo`) samodejno vrne 404 — knjižnica `WebServer` to obravnava sama, brez dodatne kode.

### Prevajanje, nalaganje, spremljanje

```bash
pio run
pio run --target upload
pio device monitor
```

### Pričakovano obnašanje

Po običajnem WiFi izpisu in IP-ju v brskalniku (na istem WiFi-ju) odpri:

| URL | Odgovor |
| --- | --- |
| `http://<esp32-ip>/`  | **Pozdravljen svet iz esp32, čšž-ji delujejo!** |
| `http://<esp32-ip>/1` | **Spletna stran enojk 111111111111 !** |
| `http://<esp32-ip>/0` | **Spletna stran ničel 000000000000 !** |
| `http://<esp32-ip>/karkoli-drugega` | 404 Not Found |
