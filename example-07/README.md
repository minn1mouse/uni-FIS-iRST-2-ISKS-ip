# Example 07 — ESP32 as a WiFi access point (soft AP)

A flip on every previous networked example. Instead of **joining** a WiFi network (station / STA mode with `WiFi.begin(...)`), the ESP32 **broadcasts its own** (access-point / AP mode with `WiFi.softAP(...)`). Your laptop or phone joins the ESP32 like it would a router, and the HTTP server is reachable at the default soft-AP IP **192.168.4.1**. No router or internet is involved — it's a self-contained local network.

Obrat v primerjavi z vsemi prejšnjimi omrežnimi primeri. Namesto da bi se ESP32 **pridružil** WiFi omrežju (postaja / STA način z `WiFi.begin(...)`), zdaj **sam oddaja** svoje omrežje (dostopna točka / AP način z `WiFi.softAP(...)`). Prenosnik ali telefon se na ESP32 priklopi, kot bi se na usmerjevalnik, in HTTP strežnik je dosegljiv na privzetem soft-AP IP-ju **192.168.4.1**. Usmerjevalnik ali internet nista vključena — gre za samostojno lokalno omrežje.

---

## English

### Prerequisites

Just the ESP32 dev board on USB. **No `secrets.h` setup needed** — this example doesn't connect to a remote network. The AP's SSID/password live as plain string literals at the top of `src/main.cpp` (default: `esp32_1` / `12345678`).

### What's new in this example

- **`WiFi.softAP(ssid, password)`** instead of `WiFi.begin(ssid, password)`. The ESP32 becomes the network — clients connect TO it.
- **`WiFi.softAPIP()`** returns the ESP32's IP on its own network (defaults to `192.168.4.1`). That's what you put in your browser.
- **AP credentials are not secrets.** The SSID is broadcast publicly the moment the AP turns on; the password gates joining but is also handed out. They don't belong in the shared `secrets.h` (which is for credentials of remote networks we connect to) — they live inline in `main.cpp`.
- **No `printWifiTarget()` call** — that helper logs the *home / hotspot* slot the build is joining, which is meaningless in AP mode.

### Trade-offs (AP vs STA)

| | Station (STA) | Access Point (AP) |
| --- | --- | --- |
| `WiFi.begin(ssid, pw)` | yes | — |
| `WiFi.softAP(ssid, pw)` | — | yes |
| ESP32 joins an existing network | ✅ | ❌ |
| ESP32 broadcasts a new network | ❌ | ✅ |
| Internet access | usually ✅ via the router | ❌ — clients only see the ESP32 |
| IP source | DHCP from the router | fixed at `192.168.4.1` |
| Reachable from off the LAN | yes (if router forwards) | no — clients must join the AP |

### Build, upload, monitor

From inside `example-07/`. Same two-step pattern (code + filesystem):

```bash
pio run --target upload       # flash the code
pio run --target uploadfs     # flash data/ as LittleFS
pio device monitor            # 115200 baud
```

### Trying it from a phone or laptop

1. After upload, the serial monitor should print:
   ```
   LittleFS priklopljen.
   Vzpostavitev programske dostopne točke na esp32 modulu.
   IP naslov esp32 dostopne točke: 192.168.4.1
   ```
2. On your phone/laptop, open the **WiFi list**. You should see **esp32_1** as an available network.
3. Connect to it with password **12345678**. (Phones may warn "no internet, stay connected?" — say yes.)
4. Open `http://192.168.4.1/` in a browser. You see the heading "Esp32 kot WiFi dostopna točka - Access Point AP!".

### Troubleshooting

- **`esp32_1` doesn't show up in the WiFi list** → board didn't boot the sketch (check power and `pio run` output), or you're scanning before the AP came up — wait a few seconds and refresh.
- **Connected to `esp32_1` but `http://192.168.4.1/` doesn't load** → browser is trying HTTPS; type `http://` explicitly. Or LittleFS isn't mounted (run `uploadfs`).
- **`Manjka /index.html`** → mount succeeded but `data/` wasn't uploaded. Re-run `uploadfs`.

---

## Slovensko

### Predpogoji

Samo razvojna ploščica ESP32 na USB-ju. **Nastavitev `secrets.h` ni potrebna** — ta primer se ne povezuje na oddaljeno omrežje. SSID/geslo AP-ja sta navadna nizovna literala na vrhu `src/main.cpp` (privzeto: `esp32_1` / `12345678`).

### Kaj je novega v tem primeru

- **`WiFi.softAP(ssid, password)`** namesto `WiFi.begin(ssid, password)`. ESP32 postane omrežje — odjemalci se povezujejo NANJ.
- **`WiFi.softAPIP()`** vrne IP ESP32 na njegovem lastnem omrežju (privzeto `192.168.4.1`). Ta IP vpišeš v brskalnik.
- **AP podatki niso skrivnost.** SSID je javno oddajan v trenutku, ko se AP vklopi; geslo nadzira pridružitev, vendar je tudi razdeljeno. Ne sodita v skupno `secrets.h` (ki je za podatke oddaljenih omrežij, na katera se povezujemo) — živita vrstično v `main.cpp`.
- **Brez klica `printWifiTarget()`** — ta pomožna funkcija izpiše *home / hotspot* gnezdo, ki ga gradnja uporablja, kar v AP načinu nima pomena.

### Razlike med AP in STA

| | Postaja (STA) | Dostopna točka (AP) |
| --- | --- | --- |
| `WiFi.begin(ssid, pw)` | da | — |
| `WiFi.softAP(ssid, pw)` | — | da |
| ESP32 se pridruži obstoječemu omrežju | ✅ | ❌ |
| ESP32 oddaja novo omrežje | ❌ | ✅ |
| Internetni dostop | običajno ✅ prek usmerjevalnika | ❌ — odjemalci vidijo le ESP32 |
| Vir IP-ja | DHCP iz usmerjevalnika | fiksen `192.168.4.1` |
| Dosegljivost izven LAN | da (če usmerjevalnik posreduje) | ne — odjemalci se morajo pridružiti AP-ju |

### Prevajanje, nalaganje, spremljanje

Iz mape `example-07/`. Enak dvostopenjski vzorec (koda + datotečni sistem):

```bash
pio run --target upload       # naloži kodo
pio run --target uploadfs     # naloži data/ kot LittleFS
pio device monitor            # 115200 baud
```

### Preizkus s telefonom ali prenosnikom

1. Po nalaganju naj serijski monitor izpiše:
   ```
   LittleFS priklopljen.
   Vzpostavitev programske dostopne točke na esp32 modulu.
   IP naslov esp32 dostopne točke: 192.168.4.1
   ```
2. Na telefonu/prenosniku odpri **seznam WiFi omrežij**. Med njimi naj se pojavi **esp32_1**.
3. Pridruži se mu z geslom **12345678**. (Telefoni lahko opozorijo "ni interneta, ostani povezan?" — potrdi.)
4. V brskalniku odpri `http://192.168.4.1/`. Pojavi se naslov "Esp32 kot WiFi dostopna točka - Access Point AP!".

### Odpravljanje težav

- **`esp32_1` se ne pojavi v seznamu WiFi** → ploščica ni zagnala skice (preveri napajanje in izpis `pio run`), ali pa iščeš pred zagonom AP — počakaj nekaj sekund in osveži.
- **Povezana sem na `esp32_1`, vendar `http://192.168.4.1/` ne naloži** → brskalnik poskuša HTTPS; vpiši izrecno `http://`. Ali pa LittleFS ni priklopljen (zaženi `uploadfs`).
- **`Manjka /index.html`** → priklop je uspel, vendar `data/` ni bila naložena. Ponovno zaženi `uploadfs`.
