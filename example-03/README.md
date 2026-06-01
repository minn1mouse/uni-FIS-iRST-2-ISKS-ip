# Example 03 — HTTP server on ESP32

Builds on [example-02](../example-02/README.md). The ESP32 connects to WiFi (same shared-secrets setup as before), then starts a tiny HTTP server on port 80. Visiting the ESP32's printed IP in a browser returns **"Pozdravljen svet iz esp32!"**.

Nadgradnja [example-02](../example-02/README.md). ESP32 se poveže na WiFi (enaka nastavitev kot prej, prek skupnih gesel), nato zažene majhen HTTP strežnik na vratih 80. Z obiskom izpisanega IP-ja v brskalniku ESP32 vrne **"Pozdravljen svet iz esp32!"**.

---

## English

### Hardware

- ESP32 dev board (`esp32dev`) — same as previous examples.
- A laptop or phone on the **same WiFi** as the ESP32, with any web browser. The IP printed on the serial monitor is the server's; clients must be on the same network to reach it.

### One-time configuration

The WiFi credentials and the `home` / `hotspot` toggle live in `shared/include/secrets.h`. If you haven't done it yet, follow the steps in [example-02/README.md](../example-02/README.md#one-time-configuration).

### What's new in this example

- `WebServer server(80)` — an HTTP server object listening on the default HTTP port.
- `void handle_root()` — a **route handler**, the function called when a client requests `/`. It calls `server.send(200, "text/html; charset=UTF-8", "...")` to return the response.
- `server.on("/", handle_root)` — registers the route during `setup()`.
- `server.begin()` — starts the server.
- `server.handleClient()` in `loop()` — must be called continuously so the server can accept and dispatch incoming requests.

### Build, upload, monitor

Run from inside `example-03/`:

```bash
pio run                    # compile
pio run --target upload    # flash to the connected ESP32
pio device monitor         # open serial monitor (115200 baud)
```

### Expected behaviour

On the serial monitor:

```
Povezovanje na hotspot omrežje: <your-ssid>
Povezovanje z WiFi omrežjem...
...
Povezava z WiFi omrežjem je vzpostavljena.
Moj IP naslov je: 192.168.x.x
```

…followed by the onboard LED double-blink. Then open `http://192.168.x.x/` in a browser on the same WiFi — the page shows:

```
Pozdravljen svet iz esp32! - Andrej Škraba
```

The `loop()` keeps `server.handleClient()` running, so the page stays reachable for as long as the ESP32 is powered.

---

## Slovensko

### Strojna oprema

- Razvojna ploščica ESP32 (`esp32dev`) — enako kot v prejšnjih primerih.
- Prenosnik ali telefon **na istem WiFi-ju** kot ESP32 s poljubnim spletnim brskalnikom. Na serijskem monitorju se izpiše IP strežnika; odjemalci morajo biti v istem omrežju, da je dosegljiv.

### Enkratna nastavitev

WiFi gesla in stikalo `home` / `hotspot` so v `shared/include/secrets.h`. Če tega še nisi naredila, sledi korakom v [example-02/README.md](../example-02/README.md#enkratna-nastavitev).

### Kaj je novega v tem primeru

- `WebServer server(80)` — objekt HTTP strežnika, ki posluša na privzetih vratih 80.
- `void handle_root()` — **funkcija za obravnavo zahteve** (route handler), ki jo strežnik pokliče, ko odjemalec zahteva pot `/`. Z `server.send(200, "text/html; charset=UTF-8", "...")` vrne odgovor.
- `server.on("/", handle_root)` — v `setup()` registriramo pot.
- `server.begin()` — zaženemo strežnik.
- `server.handleClient()` v `loop()` — funkcijo moramo klicati neprestano, da strežnik sprejme in obdela dohodne zahteve.

### Prevajanje, nalaganje, spremljanje

Vse ukaze poganjajte iz mape `example-03/`:

```bash
pio run                    # prevedi
pio run --target upload    # naloži na priključen ESP32
pio device monitor         # odpri serijski monitor (115200 baud)
```

### Pričakovano obnašanje

Na serijskem monitorju:

```
Povezovanje na hotspot omrežje: <tvoj-ssid>
Povezovanje z WiFi omrežjem...
...
Povezava z WiFi omrežjem je vzpostavljena.
Moj IP naslov je: 192.168.x.x
```

…sledi 2× utrip vgrajene LED. Nato v brskalniku na isti WiFi mreži odpri `http://192.168.x.x/` — stran prikaže:

```
Pozdravljen svet iz esp32! - Andrej Škraba
```

`loop()` neprestano kliče `server.handleClient()`, tako da je stran dosegljiva, dokler je ESP32 vklopljen.
