// EN: HTTP server bound to a **specific LAN IP** (not 0.0.0.0). The IP and
//     port come from the gitignored `shared/node/config.js` — keeps your
//     real LAN IP out of git. See `shared/node/config.example.js` for the
//     template. If you'd rather bind to every interface (so it works
//     regardless of which network you're on), pass `"0.0.0.0"` or omit
//     the hostname argument to `listen()`.
// SL: HTTP strežnik, vezan na **določen IP v lokalnem omrežju** (ne
//     0.0.0.0). IP in vrata bere iz gitignored datoteke
//     `shared/node/config.js` — tako tvoj LAN IP ostane zunaj gita. Glej
//     `shared/node/config.example.js` za predlogo. Če bi se raje vezala
//     na vse vmesnike (da deluje ne glede na trenutno omrežje), uporabi
//     `"0.0.0.0"` ali pa argumenta `hostname` v `listen()` ne podaj.
import http from "http";
import { SERVER_HOST } from "../shared/node/config.js";

const PORT = 8080;

const server = http.createServer((req, res) => {
    res.statusCode = 200;
    res.setHeader("Content-Type", "text/plain");
    res.end("Pozdravljen svet VSC v marcu!");
});

server.listen(PORT, SERVER_HOST, () => {
    console.log(`Strežnik teče na http://${SERVER_HOST}:${PORT}`);
});
