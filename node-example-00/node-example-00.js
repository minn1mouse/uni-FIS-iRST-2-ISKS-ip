// EN: Minimal HTTP server — Node.js standard library `http`, no extra
//     dependencies. Listens on port 8080 on every network interface (no
//     hostname is bound, so 0.0.0.0 is implied — reachable from this
//     machine via localhost AND from anywhere on the LAN via the LAN IP).
//     Replies "Pozdravljen svet" to every request regardless of path or
//     method.
// SL: Minimalni HTTP strežnik — Node.js standardni modul `http`, brez
//     dodatnih odvisnosti. Posluša na vratih 8080 na vseh omrežnih
//     vmesnikih (vmesnik ni izrecno določen, kar pomeni 0.0.0.0 —
//     dosegljiv s tega računalnika prek localhosta IN iz lokalnega omrežja
//     prek LAN IP-ja). Na vsako zahtevo ne glede na pot ali metodo
//     odgovori "Pozdravljen svet".
import http from 'http';

http.createServer(function (req, res) {
    res.writeHead(200, { 'Content-Type': 'text/plain' });
    res.end('Pozdravljen svet\n');
}).listen(8080);

console.log('Server running on http://0.0.0.0:8080');
