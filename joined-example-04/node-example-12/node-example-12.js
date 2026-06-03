// EN: Hub for joined-example-04. ESP32 streams potentiometer readings
//     at ~20 Hz on port 8811; Node forwards them to all browsers on
//     port 8888. Same `handleIncoming` + `broadcastTo` pattern as
//     joined-example-03 — only the per-type handler set changes:
//     `handlePotenciometer` is the new addition (forwards ESP32
//     messages to browsers). `handleLED` is kept so the LED control
//     path from joined-02/03 still works if you add a button to the
//     HTML later.
// SL: Zvezdišče za joined-example-04. ESP32 ~20-krat na sekundo
//     pošilja meritve potenciometra na vrata 8811; Node jih posreduje
//     vsem brskalnikom na vratih 8888. Enak vzorec `handleIncoming` +
//     `broadcastTo` kot v joined-example-03 — le da ima drug nabor
//     funkcij za vrste sporočil: `handlePotenciometer` je novost
//     (posreduje sporočila ESP32 brskalnikom). `handleLED` ohranimo,
//     da pot za nadzor LED iz joined-02/03 še naprej deluje, če v
//     HTML kasneje dodaš gumb.
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
    res.sendFile(path.join(__dirname, "node-example-12.html"));
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

function handleLED(msg)            { broadcastTo(wssEsp32,   JSON.stringify(msg)); }
function handlePotenciometer(msg)  { broadcastTo(wssBrowser, JSON.stringify(msg)); }

function handleIncoming(source, raw) {
    // EN: don't log every potentiometer message — at 20 Hz it would flood
    //     the console. Only log unknown/malformed payloads.
    // SL: vsakega potenciometrskega sporočila ne beležimo — pri 20 Hz bi
    //     zasul konzolo. Beležimo le neznana/napačna sporočila.
    const text = String(raw);

    let msg;
    try {
        msg = JSON.parse(text);
    } catch (err) {
        console.log(`${source}: (ni veljaven JSON: ${err.message})`);
        return;
    }

    switch (msg.tipSporočila) {
        case "LED":           handleLED(msg);           break;
        case "potenciometer": handlePotenciometer(msg); break;
        default:
            console.log(`${source}: (nepoznan tipSporočila: ${msg.tipSporočila})`);
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
