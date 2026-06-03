// EN: Hub for joined-example-04a — identical in shape to joined-04's
//     node-example-12. ESP32 streams potentiometer readings on port
//     8811; Node forwards them to all browsers on port 8888.
//     Difference vs joined-04 lives entirely in the browser HTML
//     (live line graph instead of just the latest number).
// SL: Zvezdišče za joined-example-04a — po obliki identično kot
//     node-example-12 v joined-04. ESP32 pošilja meritve potenciometra
//     na vrata 8811; Node jih posreduje vsem brskalnikom na vratih
//     8888. Razlika v primerjavi z joined-04 je le v HTML strani
//     brskalnika (živi linijski graf namesto le zadnje številke).
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
    res.sendFile(path.join(__dirname, "node-example-13.html"));
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
