// EN: Same Express + ws scaffolding as node-example-05, plus actual
//     **message exchange** over the WebSocket. Two new bits:
//     (1) Client side (in the HTML): a button + input field that calls
//         `ws.send(value)` to push a string to the server.
//     (2) Server side (here): inside the `wss.on("connection", ...)`
//         callback we now also register `ws.on("message", ...)` so the
//         server logs whatever the browser sent.
//     This is the Node mirror of example-11 on the ESP32 side — same
//     "scaffold then add UI" progression.
// SL: Enaka Express + ws infrastruktura kot v node-example-05, dodana pa
//     je dejanska **izmenjava sporočil** prek WebSocket-a. Dve novosti:
//     (1) Na strani odjemalca (v HTML-ju): gumb in vnosno polje, ki
//         pokličeta `ws.send(value)` za pošiljanje niza na strežnik.
//     (2) Na strani strežnika (tu): znotraj povratnega klica
//         `wss.on("connection", ...)` zdaj registriramo še
//         `ws.on("message", ...)`, da strežnik beleži, kar je brskalnik
//         poslal.
//     To je Node odraz example-11 na strani ESP32 — enaka delitev
//     "infrastruktura, nato vmesnik".
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
    res.sendFile(path.join(__dirname, "node-example-06.html"));
});

http.createServer(app).listen(HTTP_PORT, () => {
    console.log(`HTTP strežnik na http://${ip.address()}:${HTTP_PORT}/`);
});

const wss = new WebSocketServer({ port: WS_PORT });
wss.on("connection", (ws, req) => {
    console.log(`Brskalnik se je povezal na WebSocket (vrata ${WS_PORT}).`);

    // EN: per-client message handler. Fires every time this specific
    //     browser sends a message over its socket. `msg` is a Buffer by
    //     default; we coerce to String for the log so it's readable.
    // SL: funkcija za sporočila tega odjemalca. Sproži se ob vsakem
    //     sporočilu, ki ga ta brskalnik pošlje prek vtičnika. `msg` je
    //     privzeto Buffer; v izpisu ga pretvorimo v String, da je
    //     berljivo.
    ws.on("message", (msg) => {
        console.log(`Prejeto sporočilo: ${String(msg)}`);
    });
});

console.log(`WebSocket strežnik na ws://${ip.address()}:${WS_PORT}/`);
