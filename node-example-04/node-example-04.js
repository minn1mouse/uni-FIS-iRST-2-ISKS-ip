// EN: HTTP server using the **Express.js** framework — a thin layer over
//     Node's built-in `http` that turns route definition and static-file
//     serving into one-liners. Compare with node-example-03's manual
//     `if (req.url === "...")` dispatch and `fs.readFile` machinery: with
//     Express the same idea fits in two lines (`app.get(...)` and
//     `res.sendFile(...)`). This example also uses the `ip` package to
//     auto-discover the laptop's LAN IP at startup — no hardcoded address,
//     no shared/node/config.js needed.
// SL: HTTP strežnik z ogrodjem **Express.js** — tanko plastjo nad
//     vgrajenim Node modulom `http`, ki definicijo poti in postrežbo
//     statičnih datotek spravi v eno-vrstičnice. Primerjaj z ročnim
//     `if (req.url === "...")` razdeljevanjem in `fs.readFile` strojevjem
//     iz node-example-03: z Express je vse v dveh vrsticah
//     (`app.get(...)` in `res.sendFile(...)`). Primer uporabi tudi paket
//     `ip` za samodejno odkrivanje LAN IP-ja prenosnika ob zagonu — brez
//     trdo zakodiranega naslova, brez shared/node/config.js.
import http from "http";
import express from "express";
import ip from "ip";
import path from "path";
import { fileURLToPath } from "url";

const __dirname = path.dirname(fileURLToPath(import.meta.url));

// EN: Express creates an `app` object onto which routes are registered.
//     Passing `app` to `http.createServer` gives you a normal Node HTTP
//     server that delegates request handling to Express.
// SL: Express ustvari objekt `app`, na katerega registriramo poti.
//     Objekt `app` posredujemo `http.createServer`, da dobimo navaden
//     Node HTTP strežnik, ki obravnavo zahtev preda Expressu.
const app = express();

// EN: Port 80 is the default for HTTP, but Windows requires admin to bind
//     to it. 8080 is the common dev alternative — flip to 80 if you're on
//     Linux/macOS or running an elevated PowerShell.
// SL: Vrata 80 so privzeta za HTTP, vendar Windows zahteva skrbniške
//     pravice za vezavo. 8080 je pogosta razvijalska alternativa —
//     spremeni na 80, če si na Linuxu/macOS ali v skrbniškem PowerShellu.
const PORT = 80;

// EN: `app.get(path, handler)` registers a GET route. `res.sendFile`
//     streams a file from disk with Content-Type inferred from the
//     extension — no manual `fs.readFile` or `res.writeHead` needed.
// SL: `app.get(path, handler)` registrira GET pot. `res.sendFile` pošlje
//     datoteko z diska in iz končnice samodejno določi Content-Type —
//     ročnega `fs.readFile` ali `res.writeHead` ni treba klicati.
app.get("/", (req, res) => {
    res.sendFile(path.join(__dirname, "node-example-04.html"));
});

http.createServer(app).listen(PORT, () => {
    // EN: `ip.address()` returns the machine's LAN IP at startup, so the
    //     printed URL is always correct without hardcoding.
    // SL: `ip.address()` ob zagonu vrne LAN IP računalnika, tako da
    //     izpisani URL ne potrebuje trdo zakodirane vrednosti.
    console.log(`Strežnik zagnan na http://${ip.address()}:${PORT}/`);
});
