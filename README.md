# CYD Dual SPI

Baseline firmware for the Kecheng variant of the ESP32 Cheap Yellow Display (CYD) board.
This variant differs from the commonly documented CYD pinout and requires separate SPI bus
configuration for the display and touch controller to operate reliably.

---

## Background

On this board the XPT2046 touch controller is physically wired over the SD card module.
The standard single-bus approach causes conflicts, so the display and touch run on independent
SPI buses — VSPI for the ILI9341 display and HSPI for the touch controller. During touch reads,
the SD card CS line must be held low to avoid bus contention.

The provided `main.cpp` demonstrates dual-bus initialisation and a basic touch-to-draw example,
serving as a working baseline to build on.

---

## Hardware

Board: ESP32 CYD (Kecheng / Dual-SPI variant)
Display: ILI9341 — 320x240, landscape
Touch: XPT2046

### Pin Assignments

<table>
<tr>
<td>

**Display — VSPI**

| Signal | GPIO |
|--------|------|
| CLK    | 14   |
| MOSI   | 13   |
| MISO   | 12   |
| CS     | 15   |
| DC     | 2    |
| RST    | —    |
| BL     | 21   |

</td>
<td>

**Touch — HSPI**

| Signal | GPIO |
|--------|------|
| CLK    | 25   |
| MOSI   | 32   |
| MISO   | 39   |
| CS     | 33   |
| IRQ    | 36   |

</td>
<td>

**Other**

| Signal | GPIO | Notes                             |
|--------|------|-----------------------------------|
| LDR    | 34   | Ambient light sensor (auto-dim)   |
| BL PWM | 21   | Shared with display backlight pin |

</td>
</tr>
</table>

---

## Dependencies

| Library              | Source                                           | Version  |
|----------------------|--------------------------------------------------|----------|
| TFT_eSPI             | bodmer/TFT_eSPI                                  | ^2.5.0   |
| XPT2046_Touchscreen  | https://github.com/PaulStoffregen/XPT2046_Touchscreen | latest |

Managed via PlatformIO. See `platformio.ini` for full build configuration.

---

## Build & Flash

Requires [PlatformIO](https://platformio.org/) (CLI or VS Code extension).

```bash
# Build
pio run

# Flash
pio run --target upload

# Monitor serial output
pio device monitor --baud 115200
```

---

## Touch Calibration

Raw ADC values from the XPT2046 are mapped to screen coordinates in `main.cpp`.
The defaults work for most units but may need tuning for your specific board.

```cpp
// X: raw 200–3700  ->  screen 0–320
// Y: raw 240–3800  ->  screen 0–240
```

Adjust the `map()` calls in the touch read loop if your touch registration is off.

---

## Display Dimming (Optional)

The `CYD Display Dimming` file documents auto-brightness using the onboard LDR (GPIO 34)
and PWM on the backlight pin (GPIO 21). Key values:

| Constant        | Value | Description                            |
|-----------------|-------|----------------------------------------|
| MIN_BRIGHTNESS  | 20    | Prevents fully black backlight         |
| MAX_BRIGHTNESS  | 255   | Full brightness                        |
| SENSOR_MAX_DARK | 170   | LDR reading when fully covered         |
| Smoothing       | 0.95  | Exponential filter — reduces flickering |

Note: the backlight bleeds slightly into the LDR, so the sensor never reads true zero.
The calibration values account for this.

---

## Project Structure

```
CYD-Dual-SPI/
├── main.cpp              # Firmware — dual SPI init + touch demo
├── platformio.ini        # Build config, board, libraries
└── CYD Display Dimming   # Notes and calibration code for auto-brightness
```
