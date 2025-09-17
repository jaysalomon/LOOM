# ESP32 Firmware (Arduino)

This folder contains a standalone Arduino sketch of the LOOM consciousness demo for ESP32-S3.

- Project: `firmware/esp32/loom_esp32_demo/`
- Board: ESP32-S3 (tested on ESP32-S3-WROOM-1 N16R8)
- Features: OLED stats + RGB LED visualization (NeoPixel default, discrete fallback)

## Build and Upload

You can use Arduino IDE or Arduino CLI.

Arduino CLI (Windows PowerShell):

```powershell
# Compile
"C:\\Program Files\\arduino-ide\\resources\\app\\lib\\backend\\resources\\arduino-cli.exe" compile -b esp32:esp32:esp32s3 "E:\\My Drive\\LOOM\\firmware\\esp32\\loom_esp32_demo"

# Upload (adjust COM port if needed)
"C:\\Program Files\\arduino-ide\\resources\\app\\lib\\backend\\resources\\arduino-cli.exe" upload -b esp32:esp32:esp32s3 -p COM5 "E:\\My Drive\\LOOM\\firmware\\esp32\\loom_esp32_demo"
```

Dependencies:
- esp32 core for Arduino
- Adafruit SSD1306 (+ GFX/BusIO)
- Adafruit NeoPixel

## RGB LED Configuration

Default is NeoPixel on GPIO 48:
- `LOOM_RGB_NEOPIXEL` = 1
- `NEOPIXEL_PIN` = 48
- `NEOPIXEL_NUM_LEDS` = 1

To use a discrete RGB LED instead:
- Set `LOOM_RGB_NEOPIXEL` to 0 in `esp32_loom_arduino.h`
- Set `RGB_LED_RED_PIN`, `RGB_LED_GREEN_PIN`, `RGB_LED_BLUE_PIN`
- Set `RGB_LED_COMMON_ANODE` accordingly

## Folder Contents
- `loom_esp32_demo.ino` — main sketch
- `esp32_loom_arduino.h/.cpp` — LOOM Arduino engine and visualizations

## Notes
- The original sources remain under `kernel/esp32/arduino_project/` but this folder is the recommended entry point for Arduino users.
