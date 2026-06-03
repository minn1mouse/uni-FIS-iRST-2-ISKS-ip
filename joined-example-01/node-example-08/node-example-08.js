// EN: **Central hub** — the Node side of joined-example-01. Three things
//     in one process:
//       1. Express HTTP server on port 80, serves the browser UI page.
//       2. WebSocketServer `wssBrowser` on port 8888, accepts browser
//          clients (the page opens this socket after load).
//       3. WebSocketServer `wssEsp32` on port 8811, accepts ESP32
//          clients (the chip in ../src/main.cpp connects here).
//     The hub then **forwards** every message it receives from the
//     browser side to every connected ESP32, unchanged. The browser
//     never talks to the ESP32 directly; everything flows through Node.
// SL: **Osrednje zvezdišče** — stran Node primera joined-example-01. V
//     enem procesu tri stvari:
//       1. Express HTTP strežnik na vratih 80, postreže brskalniško
//          stran.
//       2. WebSocketServer `wssBrowser` na vratih 8888, sprejema
//          brskalniške odjemalce (stran odpre ta vtičnik po nalaganju).
//       3. WebSocketServer `wssEsp32` na vratih 8811, sprejema ESP32
//          odjemalce (čip v ../src/main.cpp se poveže tukaj).
//     Zvezdišče vsako sporočilo, prejeto od brskalnika, nespremenjeno
//     **posreduje** vsem priklopljenim ESP32. Brskalnik z ESP32 ne
//     komunicira neposredno; vse gre prek Node-a.
import http from "http";
import express from "express";
import ip from "ip";
import path from "path";
import { fileURLToPath } from "url";
import { WebSocketServer } from "ws";

const __dirname = path.dirname(fileURLToPath(import.meta.url));

const app = express();

const HTTP_PORT  = 80;
const WS_BROWSER = 8888;
const WS_ESP32   = 8811;

app.get("/", (req, res) => {
    res.sendFile(path.join(__dirname, "node-example-08.html"));
});

http.createServer(app).listen(HTTP_PORT, () => {
    console.log(`HTTP strežnik na http://${ip.address()}:${HTTP_PORT}/`);
});

const wssBrowser = new WebSocketServer({ port: WS_BROWSER });
const wssEsp32   = new WebSocketServer({ port: WS_ESP32 });

console.log(`WebSocket (brskalnik) na ws://${ip.address()}:${WS_BROWSER}/`);
console.log(`WebSocket (ESP32)     na ws://${ip.address()}:${WS_ESP32}/`);

wssEsp32.on("connection", (ws) => {
    console.log("ESP32 se je povezal (vrata 8811).");
    ws.on("close", () => console.log("ESP32 se je odklopil."));
});

wssBrowser.on("connection", (ws) => {
    console.log("Brskalnik se je povezal (vrata 8888).");

    // EN: forward every message from this browser to every ESP32. The
    //     hub does not parse, validate, or modify the payload — that's
    //     the ESP32's job (it expects JSON of the homework-03 shape).
    //     `wssEsp32.clients` is the live Set of currently-connected
    //     ESP32 sockets.
    // SL: vsako sporočilo s tega brskalnika posreduj vsem ESP32.
    //     Zvezdišče sporočila ne razčlenjuje, ne preverja in ne
    //     spreminja — to je naloga ESP32 (pričakuje JSON oblike kot v
    //     homework-03). `wssEsp32.clients` je živa množica trenutno
    //     priklopljenih ESP32 vtičnikov.
    ws.on("message", (raw) => {
        const text = String(raw);
        console.log(`Brskalnik → ESP32: ${text}`);
        wssEsp32.clients.forEach((esp) => {
            if (esp.readyState === esp.OPEN) {
                esp.send(text);
            }
        });
    });
});
