// EN: Express HTTP server + `ws` WebSocket server side-by-side on
//     different ports. Two new concepts vs node-example-04:
//     (1) The `ws` package adds a separate WebSocket server (port 8888)
//         alongside the Express HTTP server (port 8080), so the browser
//         can keep a persistent bidirectional connection open after
//         loading the page.
//     (2) `wss.on("connection", ...)` logs whenever a browser opens the
//         socket — server-side proof the connection is live.
//     This file sets up the plumbing; the HTML opens the socket on page
//     load but doesn't exchange any messages yet. Sending/receiving
//     comes in later examples (the same pedagogical split as ESP32-side
//     example-10 → example-11).
// SL: Express HTTP strežnik + `ws` WebSocket strežnik vzporedno na
//     različnih vratih. Dve novosti v primerjavi z node-example-04:
//     (1) Paket `ws` doda ločen WebSocket strežnik (vrata 8888) poleg
//         Express HTTP strežnika (vrata 8080), tako da brskalnik po
//         nalaganju strani ohranja trajno dvosmerno povezavo.
//     (2) `wss.on("connection", ...)` ob vsaki priključitvi brskalnika
//         na vtičnik to zabeleži — strežniški dokaz, da je povezava
//         živa.
//     Ta datoteka postavi infrastrukturo; HTML ob nalaganju odpre
//     vtičnik, vendar sporočil še ne izmenjuje. Pošiljanje/sprejemanje
//     pride v naslednjih primerih (enaka delitev kot na strani ESP32
//     med example-10 → example-11).
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
    res.sendFile(path.join(__dirname, "node-example-05.html"));
});

http.createServer(app).listen(HTTP_PORT, () => {
    console.log(`HTTP strežnik na http://${ip.address()}:${HTTP_PORT}/`);
});

// EN: separate WebSocket server on its own port. Each browser that opens
//     `ws://<host>:8888/` triggers the "connection" callback once.
// SL: ločen WebSocket strežnik na svojih vratih. Vsak brskalnik, ki odpre
//     `ws://<host>:8888/`, sproži povratni klic "connection" enkrat.
const wss = new WebSocketServer({ port: WS_PORT });
wss.on("connection", (ws, req) => {
    console.log(`Brskalnik se je z dvojno povezavo povezal na WebSocket (vrata ${WS_PORT}).`);
});

console.log(`WebSocket strežnik na ws://${ip.address()}:${WS_PORT}/`);
