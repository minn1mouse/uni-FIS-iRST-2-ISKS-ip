// EN: Bidirectional hub for joined-example-04 — identical in shape to
//     joined-example-03's. Two parallel WebSocketServers (8888 for the
//     browser, 8811 for the ESP32), one `handleIncoming` parser/router
//     that dispatches by `tipSporočila` to per-type handlers, and
//     per-direction broadcasts. Node code is unchanged from joined-03;
//     the only thing different in this example is the browser HTML
//     (canvas visualisation of `tipka` events).
// SL: Dvosmerno zvezdišče za joined-example-04 — po obliki identično
//     kot v joined-example-03. Dva vzporedna WebSocketServerja (8888
//     za brskalnik, 8811 za ESP32), ena funkcija `handleIncoming`, ki
//     razčleni in usmerja po `tipSporočila` v funkcije za posamezno
//     vrsto, ter razpošiljanje po smereh. Node koda je nespremenjena;
//     edina razlika v tem primeru je HTML strani brskalnika (vizualni
//     prikaz dogodkov `tipka` na platnu).
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
    res.sendFile(path.join(__dirname, "node-example-11.html"));
});

http.createServer(app).listen(HTTP_PORT, () => {
    console.log(`HTTP strežnik na http://${ip.address()}:${HTTP_PORT}/`);
});

const wssBrowser = new WebSocketServer({ port: WS_BROWSER });
const wssEsp32   = new WebSocketServer({ port: WS_ESP32 });

console.log(`WebSocket (brskalnik) na ws://${ip.address()}:${WS_BROWSER}/`);
console.log(`WebSocket (ESP32)     na ws://${ip.address()}:${WS_ESP32}/`);

function broadcastTo(server, payload) {
    server.clients.forEach((client) => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(payload);
        }
    });
}

function handleLED(msg)   { broadcastTo(wssEsp32,   JSON.stringify(msg)); }
function handleTipka(msg) { broadcastTo(wssBrowser, JSON.stringify(msg)); }

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
