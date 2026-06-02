// EN: Per-developer Node.js configuration. Copy this file to `config.js`
//     (next to it) and fill in your real values. `config.js` is gitignored
//     and will never be committed. Mirrors the C++ secrets.h pattern.
// SL: Razvijalske Node.js nastavitve. Kopiraj to datoteko v `config.js`
//     (poleg te) in vpiši prave vrednosti. `config.js` je gitignored in se
//     nikoli ne objavi. Podobno kot pri C++ datoteki secrets.h.

// EN: LAN IP of your laptop / dev machine — the address other devices on
//     the same network (ESP32, phone, another laptop) use to reach Node
//     servers running here. Find it with `ipconfig` on Windows. Port is
//     intentionally NOT here — different examples bind different ports
//     (HTTP examples use 8080, WebSocket examples use 8811, etc.), so each
//     example declares its own port inline.
// SL: LAN IP tvojega prenosnika / razvojnega računalnika — naslov, prek
//     katerega te dosegajo druge naprave v istem omrežju (ESP32, telefon,
//     drug prenosnik). Najdeš ga z `ipconfig` na Windows. Vrata namenoma
//     NISO tukaj — različni primeri uporabljajo različna vrata (HTTP
//     primeri 8080, WebSocket 8811 itd.), zato vsak primer določi svoja
//     vrata vrstično.
export const SERVER_HOST = "192.168.0.100";
