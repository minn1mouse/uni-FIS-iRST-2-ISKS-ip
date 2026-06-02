// EN: HTTP server bound to a **specific LAN IP** (not 0.0.0.0). This makes
//     the binding explicit — the server only accepts connections coming in
//     on the network interface that holds this IP. Anyone on the same LAN
//     (your ESP32, your phone, another laptop) can reach it via
//     http://<this-ip>:<port>/. Replace `hostname` with whatever IP your
//     machine has on the LAN — find it with `ipconfig` on Windows. If
//     you'd rather bind to every interface (so it works regardless of
//     which network you're on), pass `"0.0.0.0"` or omit the hostname
//     argument to `listen()`.
// SL: HTTP strežnik, vezan na **določen IP v lokalnem omrežju** (ne
//     0.0.0.0). Vezava je s tem eksplicitna — strežnik sprejema povezave
//     samo na vmesniku, ki ima ta IP. Vsi v istem LAN-u (tvoj ESP32,
//     telefon, drug prenosnik) ga dosežejo prek http://<ta-ip>:<vrata>/.
//     Spremenljivko `hostname` zamenjaj z IP-jem, ki ga ima tvoj
//     računalnik v LAN-u — najdeš ga z `ipconfig` na Windows. Če bi se
//     raje vezala na vse vmesnike (da deluje ne glede na trenutno
//     omrežje), uporabi `"0.0.0.0"` ali pa argumenta `hostname` v
//     `listen()` ne podaj.
import http from "http";

const hostname = "192.168.0.145";  // ← LAN IP of THIS machine
const port = 8080;

const server = http.createServer((req, res) => {
    res.statusCode = 200;
    res.setHeader("Content-Type", "text/plain");
    res.end("Pozdravljen svet VSC v marcu!");
});

server.listen(port, hostname, () => {
    // EN: template literal — backticks let you interpolate `${var}` directly.
    //     Backtick on Slovenian keyboard: Alt Gr + 7.
    // SL: predloga niza — povratni narekovaji omogočajo vstavljanje `${var}`.
    //     Povratni narekovaj na slovenski tipkovnici: Alt Gr + 7.
    console.log(`Strežnik teče na http://${hostname}:${port}`);
});
