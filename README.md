# ad-screen-ticker

Ad-rotation firmware for a 16x2 HD44780 LCD, written in modern C++20 for the
ATmega328p (Arduino Uno class hardware). Advertisers who pay more appear more
often — selection is a paid-weighted lottery, not a fixed playlist.

**Demo:** [Wokwi simulation](https://wokwi.com/projects/445497618841194497) —
runs the compiled firmware in your browser, no hardware needed.

## What it does

The device runs an endless display cycle on an LCD hooked up to an AVR
microcontroller:

1. Shows the advertiser's company name (5 s)
2. Shows one of that advertiser's messages (10 s, plain or blinking)
3. Picks the **next advertiser at random, weighted by how much each one paid**

The bundled catalog is a demo: five fictional advertisers (car dealer, pie
shop, car wash, burger joint, gym) with 10 messages between them. The catalog
is plain static data — swap `src/ad_catalog.cpp` and nothing else changes.

## How the weighted lottery works

Each advertiser holds `paid_amount` "tickets" out of a global total. On every
cycle the engine draws one ticket:

```
ticket = lcg_random() % total_paid          // one number in [0, total_paid)
then walk advertisers accumulating paid_amount until ticket < cumulative
```

Result: an advertiser that paid 6700 out of 20850 wins ~32% of the slots,
one that paid 1750 wins ~8% — on average, with real run-to-run variance.
The RNG is a classic [linear congruential generator](https://en.wikipedia.org/wiki/Linear_congruential_generator)
(Numerical Recipes constants, 1664525 / 1013904223) seeded from the millis
timer at boot, so every power-up produces a different rotation. Within a
single advertiser, messages rotate round-robin so repeats only happen after
the full list was shown.

## Architecture

```
main.cpp
  └─ AppController          display-cycle state machine (name → message → next)
       ├─ AdEngine          weighted lottery + per-advertiser message rotation
       ├─ ad_catalog        static catalog data (advertisers, messages)
       └─ hd44780           4-bit mode LCD driver (init, text, blink, wrap)
             └─ millis      1 ms tick from Timer2 (third-party, see below)
```

Each layer only knows the one below it. `AdEngine` is pure logic — no hardware
types, no I/O — which keeps the selection algorithm testable on a desktop
host if you want to take it there.

## Hardware

| Part | Notes |
|---|---|
| ATmega328p | Arduino Uno or bare DIP-28 on a breadboard |
| HD44780 16x2 LCD | standard parallel interface, 4-bit mode |
| 10k potentiometer | LCD contrast |
| 220R resistor | LCD backlight (if not on a module with one) |

### Wiring

LCD pins are defined in `include/lcd_driver.hpp`:

| LCD | ATmega328p |
|---|---|
| RS | PD2 |
| E  | PD3 |
| DB4 | PD4 |
| DB5 | PD5 |
| DB6 | PD6 |
| DB7 | PD7 |
| R/W | GND |
| VSS, V0 | GND (V0 through pot) |
| VDD | 5 V |

## Build & flash

Requires `avr-gcc` (which ships `avr-g++`), `avr-libc`, `binutils-avr`, and
`avrdude`. On Arch: `pacman -S avr-gcc avr-libc binutils-avr avrdude`.

```sh
make                # build ad-screen-ticker.hex + print flash/RAM usage
make flash          # program an Uno over USB (uses avrdude, arduino protocol)
make clean
```

`make flash` assumes an Arduino Uno on `/dev/ttyACM0`. For other setups,
override on the command line:

```sh
make flash PROGRAMMER=usbasp PORT=/dev/ttyUSB0
```

## Run it in the simulator (no hardware)

[Wokwi](https://wokwi.com/projects/445497618841194497) is a browser-based AVR
simulator — the shared project boots straight into the ad rotation, LCD included.
The simulator config is versioned in [`sim/`](sim/): `diagram.json` (wiring) and
`sketch.ino` (workflow notes).

To run a fresh build instead of the uploaded one:

1. `make` — produces `ad-screen-ticker.hex`
2. Open the Wokwi project and press **F1**
3. Pick **Upload Firmware and Start Simulation…** and select the `.hex` file

## Project layout

```
main.cpp            wiring the layers together + entropy seed from millis
include/            headers, one per module
src/                implementations
  ad_catalog.cpp    demo catalog — the only file to edit to change content
  ad_engine.cpp     lottery + round-robin logic
  app_controller.cpp  display cycle timing and fallbacks
  lcd_driver.cpp    HD44780 4-bit driver
  millis.cpp        third-party millisecond tracker (see below)
sim/                Wokwi simulator config (diagram + sketch notes)
Makefile            build + flash targets
```

## Design decisions

- **C++20 on an 8-bit MCU** — `constexpr` catalogs land in flash (`PROGMEM`
  territory achieved for free with the right linker setup), `enum class` for
  effect flags, no vtables needed.
- **Zero heap, no exceptions, no RTTI** — `-fno-exceptions -fno-rtti`, every
  allocation is static or on the stack. Bounded state: `kMaxAdvertisers = 8`
  caps the round-robin index array at compile time.
- **Own LCG instead of `rand()`** — `rand()` on AVR drags in ~1 KB of code for
  a non-thread-safe modulo-heavy path; a 4-byte LCG is ~20 instructions, and
  the constants are well-studied.
- **Defensive by habit** — every public entry point null-checks and clamps:
  null advertiser name falls back to "Unknown company", null message text to
  "Message unavailable", empty catalog shows "Catalog error" instead of
  undefined behavior. On a device with no console and no debugger attached,
  graceful degradation is the only error handling you get.
- **Blocking waits are a choice, not an accident** — the display cycle is
  strictly sequential (name → message → next), so busy-waiting on millis is
  honest and keeps the whole system single-tasking. If buttons or a second
  screen ever land, `AppController` is the only module that needs a scheduler.

## Third-party code

`src/millis.cpp` and `include/millis.h` are from Zak Kemble's
[millis library for AVR](http://blog.zakkemble.net/millisecond-tracking-library-for-avr/),
Copyright (C) 2018 Zak Kemble, dual-licensed GPLv3 / MIT. The MIT option is
used here; original headers kept in both files.

## License

[MIT](LICENSE) — Copyright (c) 2026 Alejandro Martínez
