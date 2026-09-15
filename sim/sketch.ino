/*
 * ad-screen-ticker — AVR advertising-screen firmware
 * https://github.com/AlejandroGML/ad-screen-ticker
 *
 * This simulator runs the compiled firmware, so this tab only documents the
 * workflow. The wiring lives in diagram.json: HD44780 in 4-bit mode on D2-D7.
 *
 * Rebuilding and loading the firmware:
 *   1. In the repo: `make`  ->  produces ad-screen-ticker.hex
 *   2. Here in Wokwi: press F1
 *   3. Pick "Upload Firmware and Start Simulation..."
 *   4. Select ad-screen-ticker.hex — the simulation boots automatically
 */
