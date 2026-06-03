// EN: Bidirectional hub for joined-example-03. Same wssBrowser (8888) +
//     wssEsp32 (8811) setup as joined-example-02, plus a `message`
//     handler on the ESP32 side so the chip can push events back. The
//     full traffic flow:
//       Browser → Node:  {"tipSporočila":"LED",   ...}  → forward to ESP32
//       ESP32   → Node:  {"tipSporočila":"tipka", ...}  → forward to Browser
//     Each direction has its own per-type router; the generic
//     `broadcastTo(server, payload)` helper takes care of the
//     "send to every open client" loop.
// SL: Dvosmerno zvezdišče za joined-example-03. Enaka nastavitev kot v
//     joined-example-02 (wssBrowser 8888 + wssEsp32 8811), poleg tega pa
//     še funkcija za sporočila na strani ESP32, da lahko čip pošilja
//     dogodke nazaj. Celoten potek prometa:
//       Brskalnik → Node:  {"tipSporočila":"LED",   ...}  → posredovanje na ESP32
//       ESP32     → Node:  {"tipSporočila":"tipka", ...}  → posredovanje brskalnikom
//     Vsaka smer ima svoj usmerjevalnik; generična pomožna funkcija
//     `broadcastTo(server, payload)` skrbi za zanko "pošlji vsem
//     odprtim odjemalcem".
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
    res.sendFile(path.join(__dirname, "node-example-10.html"));
});

http.createServer(app).listen(HTTP_PORT, () => {
    console.log(`HTTP strežnik na http://${ip.address()}:${HTTP_PORT}/`);
});

const wssBrowser = new WebSocketServer({ port: WS_BROWSER });
const wssEsp32   = new WebSocketServer({ port: WS_ESP32 });

console.log(`WebSocket (brskalnik) na ws://${ip.address()}:${WS_BROWSER}/`);
console.log(`WebSocket (ESP32)     na ws://${ip.address()}:${WS_ESP32}/`);

// EN: generic broadcast — send `payload` to every open client on `server`.
//     Used by both `handleLED` (browser → ESP32) and `handleTipka`
//     (ESP32 → browsers).
// SL: generično razpošiljanje — pošlji `payload` vsem odprtim
//     odjemalcem `server`-ja. Uporabljata jo `handleLED` (brskalnik →
//     ESP32) in `handleTipka` (ESP32 → brskalniki).
function broadcastTo(server, payload) {
    server.clients.forEach((client) => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(payload);
        }
    });
}

function handleLED(msg)   { broadcastTo(wssEsp32,   JSON.stringify(msg)); }
function handleTipka(msg) { broadcastTo(wssBrowser, JSON.stringify(msg)); }

// EN: parse + route incoming messages from any direction. Same shape on
//     both sides; only the target differs.
// SL: razčlenitev in usmerjanje dohodnih sporočil iz katerekoli smeri.
//     Enaka oblika na obeh straneh; razlika je le v cilju.
function handleIncoming(source, raw) {
    const text = String(raw);
    console.log(`${source}: ${text}`);

    let msg;
    try {
        msg = JSON.parse(text);
    } catch (err) {
        console.log(`  (ni veljaven JSON: ${err.message})`);
        return;
    }

    switch (msg.tipSporočila) {
        case "LED":   handleLED(msg);   break;
        case "tipka": handleTipka(msg); break;
        default:
            console.log(`  (nepoznan tipSporočila: ${msg.tipSporočila})`);
    }
}

wssBrowser.on("connection", (ws) => {
    console.log("Brskalnik se je povezal (vrata 8888).");
    ws.on("message", (raw) => handleIncoming("Brskalnik → Node", raw));
});

wssEsp32.on("connection", (ws) => {
    console.log("ESP32 se je povezal (vrata 8811).");
    ws.on("close", () => console.log("ESP32 se je odklopil."));
    ws.on("message", (raw) => handleIncoming("ESP32 → Node", raw));
});
