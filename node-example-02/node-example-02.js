// EN: HTTP server that serves a static HTML file from disk. Same single
//     route as node-example-01, but instead of replying with an inline
//     string the server now reads `node-example-02.html` (next to this
//     script) with `fs.readFile` and pipes the bytes into the response.
//     This is the first step toward serving real web pages.
// SL: HTTP strežnik, ki postreže statično HTML datoteko z diska. Enaka
//     ena pot kot v node-example-01, vendar strežnik namesto z vrstičnim
//     nizom odgovori z vsebino datoteke `node-example-02.html` (poleg te
//     skripte), ki jo prebere z `fs.readFile`. Prvi korak proti
//     postrežbi pravih spletnih strani.
import http from "http";

// EN: Node's built-in filesystem library — async file I/O.
// SL: Vgrajena Node knjižnica za delo z datotekami — asinhroni file I/O.
import fs from "fs";

import { SERVER_HOST } from "../shared/node/config.js";

const PORT = 8080;

const server = http.createServer((req, res) => {
    fs.readFile("node-example-02.html", (err, data) => {
        if (err) {
            // EN: 500 = internal server error — used when the file read fails.
            // SL: 500 = napaka strežnika — uporabimo, ko branje datoteke ne uspe.
            res.statusCode = 500;
            res.setHeader("Content-Type", "text/plain");
            res.end("Napaka pri branju datoteke.");
        } else {
            res.statusCode = 200;
            res.setHeader("Content-Type", "text/html");
            res.end(data);
        }
    });
});

server.listen(PORT, SERVER_HOST, () => {
    console.log(`Strežnik teče na http://${SERVER_HOST}:${PORT}/`);
});
