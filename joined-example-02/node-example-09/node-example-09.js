// EN: Central hub for joined-example-02. Same role as joined-example-01,
//     but Node now **understands the protocol** instead of acting as a
//     dumb byte relay:
//       1. Incoming browser messages are JSON.parsed up front. Bad JSON
//          is logged and dropped — never reaches the chip.
//       2. The parsed object is routed by `tipSporočila` to a per-type
//          handler. Currently only "LED" is handled — it broadcasts the
//          same JSON unchanged to all connected ESP32 clients.
//     Adding a new message type means adding a new `case` and a new
//     handler function. The chip's `tipSporočila == "LED"` check still
//     works because Node forwards the original JSON verbatim.
// SL: Osrednje zvezdišče za joined-example-02. Enaka vloga kot v
//     joined-example-01, le da Node zdaj **razume protokol** in ni
//     zgolj posrednik bajtov:
//       1. Dohodna brskalniška sporočila najprej z JSON.parse razčlenimo.
//          Napačen JSON zabeležimo in zavržemo — do čipa ne pride.
//       2. Razčlenjen objekt prek `tipSporočila` usmerimo v funkcijo za
//          posamezno vrsto. Trenutno je obravnavan samo "LED" — isti
//          JSON nespremenjen razpošlje vsem priklopljenim ESP32.
//     Dodajanje nove vrste sporočil = nov `case` + nova funkcija. Čipov
//     pregled `tipSporočila == "LED"` še naprej deluje, ker Node
//     izvirni JSON posreduje nespremenjenim.
import http from "http";
import express from "express";
import ip from "ip";
import path from "path";
import { fileURLToPath } from "url";
import { WebSocketServer, WebSocket } from "ws";

const __dirname = path.dirname(fileURLToPath(import.meta.url));

const app = express();

const HTTP_PORT  = 80;
const WS_BROWSER = 8888;
const WS_ESP32   = 8811;

app.get("/", (req, res) => {
    res.sendFile(path.join(__dirname, "node-example-09.html"));
});

http.createServer(app).listen(HTTP_PORT, () => {
    console.log(`HTTP strežnik na http://${ip.address()}:${HTTP_PORT}/`);
});

const wssBrowser = new WebSocketServer({ port: WS_BROWSER });
const wssEsp32   = new WebSocketServer({ port: WS_ESP32 });

console.log(`WebSocket (brskalnik) na ws://${ip.address()}:${WS_BROWSER}/`);
console.log(`WebSocket (ESP32)     na ws://${ip.address()}:${WS_ESP32}/`);

// EN: send `payload` (a string) to every connected ESP32 whose socket is
//     currently open. Used by per-type handlers below.
// SL: pošlji `payload` (niz) vsem priklopljenim ESP32, katerih vtičnik
//     je odprt. Uporabljajo ga spodnje funkcije po vrstah sporočil.
function broadcastToEsp32(payload) {
    wssEsp32.clients.forEach((esp) => {
        if (esp.readyState === WebSocket.OPEN) {
            esp.send(payload);
        }
    });
}

// EN: per-message-type handlers. Add more as the protocol grows.
// SL: funkcije po vrstah sporočil. Z rastjo protokola dodajaš nove.
function handleLED(msg) {
    broadcastToEsp32(JSON.stringify(msg));
}

wssBrowser.on("connection", (ws) => {
    console.log("Brskalnik se je povezal (vrata 8888).");

    ws.on("message", (raw) => {
        const text = String(raw);
        console.log(`Brskalnik → Node: ${text}`);

        let msg;
        try {
            msg = JSON.parse(text);
        } catch (err) {
            console.log(`  (ni veljaven JSON: ${err.message})`);
            return;
        }

        switch (msg.tipSporočila) {
            case "LED":
                handleLED(msg);
                break;
            default:
                console.log(`  (nepoznan tipSporočila: ${msg.tipSporočila})`);
        }
    });
});

wssEsp32.on("connection", (ws) => {
    console.log("ESP32 se je povezal (vrata 8811).");
    ws.on("close", () => console.log("ESP32 se je odklopil."));
});
