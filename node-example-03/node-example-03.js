// EN: HTTP server with **multiple routes**, each serving a different HTML
//     file. Builds on node-example-02 (single file + fs.readFile) by adding
//     URL dispatch in the request handler and using `path.join(__dirname,
//     ...)` so file paths are absolute — meaning the script works no matter
//     which directory you run it from.
// SL: HTTP strežnik z **več potmi**, vsaka postreže drugo HTML datoteko.
//     Nadgradnja node-example-02 (ena datoteka + fs.readFile) z razdelitvijo
//     URL-jev v funkciji zahteve in uporabo `path.join(__dirname, ...)`, da
//     so poti do datotek absolutne — skripta deluje ne glede na to, iz
//     katerega imenika jo zaženeš.
import http from "http";
import fs from "fs";
import path from "path";
import { fileURLToPath } from "url";
import { SERVER_HOST } from "../shared/node/config.js";

const PORT = 8080;

// EN: ES modules don't have CommonJS's `__dirname` / `__filename` globals.
//     We reconstruct them: `import.meta.url` is a file:// URL pointing at
//     this script; `fileURLToPath` turns it into an OS-native path; then
//     `path.dirname` strips the filename so we're left with just the
//     folder.
// SL: ES moduli ne poznajo CommonJS-jevih globalov `__dirname` /
//     `__filename`. Ju rekonstruiramo: `import.meta.url` je file:// URL
//     do te skripte; `fileURLToPath` ga pretvori v pot po OS pravilih;
//     `path.dirname` odstrani ime datoteke in pusti samo mapo.
const __filename = fileURLToPath(import.meta.url);
const __dirname  = path.dirname(__filename);

// EN: tiny helper — read an HTML file from this script's folder and send
//     it as the HTTP response. 200 + text/html on success, 500 on read
//     error. Same pattern as node-example-02, just factored out so the
//     three route bodies don't repeat themselves.
// SL: pomožna funkcija — preberi HTML datoteko iz mape te skripte in jo
//     pošlji kot HTTP odgovor. 200 + text/html ob uspehu, 500 ob napaki
//     branja. Enak vzorec kot v node-example-02, samo izvlečen, da se trije
//     deli za poti ne ponavljajo.
function serveFile(res, filename) {
    fs.readFile(path.join(__dirname, filename), (err, data) => {
        if (err) {
            res.writeHead(500, { "Content-Type": "text/plain; charset=utf-8" });
            res.end(`Napaka pri branju datoteke ${filename}.`);
        } else {
            res.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });
            res.end(data);
        }
    });
}

const server = http.createServer((req, res) => {
    if (req.url === "/")            serveFile(res, "node-example-03.html");
    else if (req.url === "/stranx") serveFile(res, "node-example-03x.html");
    else if (req.url === "/strany") serveFile(res, "node-example-03y.html");
    else {
        res.writeHead(404, { "Content-Type": "text/plain; charset=utf-8" });
        res.end(`Ni take poti: ${req.url}`);
    }
});

server.listen(PORT, SERVER_HOST, () => {
    console.log(`Strežnik teče na http://${SERVER_HOST}:${PORT}/`);
});
