#include <Arduino.h>

void setup() {
  // EN: setup code — runs once when the device boots.
  // SL: koda za nastavitve — izvede se enkrat, ko se naprava zažene.

  // EN: configure GPIO 2 as a digital output (drives the onboard LED).
  // SL: nožico (pin) 2 nastavimo kot digitalni izhod (krmili vgrajeno LED).
  pinMode(2, OUTPUT);
}

void loop() {
  // EN: main code — runs repeatedly while the ESP32 is powered.
  // SL: glavna koda — izvaja se ponavljajoče, dokler je ESP32 vklopljen.

  // EN: write HIGH on pin 2 — the onboard LED turns on.
  // SL: na nožici 2 zapišemo visoko vrednost (HIGH) — vgrajena LED se prižge.
  digitalWrite(2, HIGH);

  // EN: pause for the given number of milliseconds, here 500 ms (0.5 s).
  // SL: zakasnitev za podan čas v milisekundah, v tem primeru 500 ms (0.5 s).
  delay(500);

  // EN: write LOW on pin 2 — the onboard LED turns off.
  // SL: na nožici 2 zapišemo nizko vrednost (LOW) — vgrajena LED se ugasne.
  digitalWrite(2, LOW);

  // EN: pause for the given number of milliseconds, here 500 ms (0.5 s).
  // SL: zakasnitev za podan čas v milisekundah, v tem primeru 500 ms (0.5 s).
  delay(500);
}
