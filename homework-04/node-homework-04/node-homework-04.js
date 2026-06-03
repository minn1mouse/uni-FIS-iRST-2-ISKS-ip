// EN: Hub for homework-04. Same `handleIncoming` + `broadcastTo`
//     pattern as the joined-example-* hubs. Browser sends `LED`
//     messages (button clicks) → forwarded to the ESP32 on port 8811.
//     ESP32 streams `fotoupornik` readings → forwarded to all
//     browsers on port 8888. Step 3 will add a `status` message type
//     that the chip emits when intention and sensor disagree.
// SL: Zvezdišče za homework-04. Enak vzorec `handleIncoming` +
//     `broadcastTo` kot v hub-ih joined-example-*. Brskalnik pošlje
//     sporočila `LED` (kliki gumbov) → posredovano ESP32-ju na vrata
//     8811. ESP32 pošilja meritve `fotoupornik` → posredovane vsem
//     brskalnikom na vrata 8888. Korak 3 bo dodal sporočilo `status`,
//     ki ga čip pošlje, ko se ukaz in senzor razhajata.
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
    res.sendFile(path.join(__dirname, "node-homework-04.html"));
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

// EN: cache the most recent `status` message so we can replay it to
//     newly-connected browsers. The chip only emits status on state
//     CHANGE, so a browser that opens after the chip has already
//     reported its current state would otherwise see "Čakam na stanje …"
//     until something changes.
// SL: shranimo zadnje `status` sporočilo, da ga lahko ob novi povezavi
//     brskalnika ponovno pošljemo. Čip pošlje status le ob SPREMEMBI
//     stanja, zato bi brskalnik, odprt po tem, ko je čip že javil
//     trenutno stanje, sicer obtičal pri "Čakam na stanje …" dokler
//     se kaj ne spremeni.
let lastStatus = null;

function handleLED(msg)          { broadcastTo(wssEsp32,   JSON.stringify(msg)); }
function handleFotoupornik(msg)  { broadcastTo(wssBrowser, JSON.stringify(msg)); }
function handleStatus(msg)       {
    lastStatus = msg;
    console.log(`Status: ${msg.stanje}`);
    broadcastTo(wssBrowser, JSON.stringify(msg));
}

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
        case "LED":          handleLED(msg);          break;
        case "fotoupornik":  handleFotoupornik(msg);  break;
        case "status":       handleStatus(msg);       break;
        default:
            console.log(`${source}: (nepoznan tipSporočila: ${msg.tipSporočila})`);
    }
}

wssBrowser.on("connection", (ws) => {
    console.log("Brskalnik se je povezal (vrata 8888).");
    if (lastStatus) {
        ws.send(JSON.stringify(lastStatus));
    }
    ws.on("message", (raw) => handleIncoming("Brskalnik → Node", raw));
});

wssEsp32.on("connection", (ws) => {
    console.log("ESP32 se je povezal (vrata 8811).");
    ws.on("close", () => console.log("ESP32 se je odklopil."));
    ws.on("message", (raw) => handleIncoming("ESP32 → Node", raw));
});
