// EN: Same Express + ws scaffold as node-example-06, but the server now
//     treats incoming messages as **JSON** instead of raw strings.
//     `JSON.parse(msg)` turns the string into a plain object, and the
//     code can read structured fields (`msg.tipSporočila`, `msg.pin`,
//     `msg.vrednost`) — the exact same wire format homework-03 uses,
//     except there the ESP32 was the reader and Node was the sender;
//     here roles are flipped (browser sends, Node parses).
// SL: Enaka infrastruktura Express + ws kot v node-example-06, le da
//     strežnik dohodna sporočila zdaj obravnava kot **JSON** namesto
//     surovih nizov. `JSON.parse(msg)` niz spremeni v navaden objekt,
//     koda pa lahko bere strukturirana polja (`msg.tipSporočila`,
//     `msg.pin`, `msg.vrednost`) — natanko isti format kot v
//     homework-03, le da je tam ESP32 bral, Node pa pošiljal; tukaj sta
//     vlogi obrnjeni (brskalnik pošlje, Node razčleni).
import http from "http";
import express from "express";
import ip from "ip";
import path from "path";
import { fileURLToPath } from "url";
import { WebSocketServer } from "ws";

const __dirname = path.dirname(fileURLToPath(import.meta.url));

const app = express();

const HTTP_PORT = 80;
const WS_PORT   = 8888;

app.get("/", (req, res) => {
    res.sendFile(path.join(__dirname, "node-example-07.html"));
});

http.createServer(app).listen(HTTP_PORT, () => {
    console.log(`HTTP strežnik na http://${ip.address()}:${HTTP_PORT}/`);
});

const wss = new WebSocketServer({ port: WS_PORT });
wss.on("connection", (ws, req) => {
    console.log(`Brskalnik se je povezal na WebSocket (vrata ${WS_PORT}).`);

    ws.on("message", (raw) => {
        // EN: `raw` is a Buffer; coerce to String so it's readable, then
        //     parse the JSON. If the client sends malformed JSON,
        //     JSON.parse throws — wrap in try/catch so one bad message
        //     doesn't crash the server.
        // SL: `raw` je Buffer; pretvorimo ga v String, da je berljiv, in
        //     nato razčlenimo JSON. Če odjemalec pošlje napačen JSON,
        //     JSON.parse vrže napako — ovijemo v try/catch, da eno slabo
        //     sporočilo ne zruši strežnika.
        const msgString = String(raw);
        console.log(`Prejeto sporočilo: ${msgString}`);

        try {
            const msg = JSON.parse(msgString);
            console.log(`  tipSporočila: ${msg.tipSporočila}`);
            console.log(`  pin:          ${msg.pin}`);
            console.log(`  vrednost:     ${msg.vrednost}`);
        } catch (err) {
            console.log(`  (sporočilo ni veljaven JSON: ${err.message})`);
        }
    });
});

console.log(`WebSocket strežnik na ws://${ip.address()}:${WS_PORT}/`);
