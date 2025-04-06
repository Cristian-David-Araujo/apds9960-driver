# APDS9960 Driver for ESP32-S3

This is a lightweight C driver for the **APDS9960** sensor, designed for the **ESP32-S3** platform.  
It supports gesture recognition, color sensing, ambient light, and proximity detection.

> **Note**: This library uses a **Hardware Abstraction Layer (HAL)** for I2C communication, defined in `platform_esp32s3.h`. You can adapt this to your specific hardware platform as needed.

## Features

- Color sensing (Red, Green, Blue, Clear)
- Ambient light and proximity sensing
- Designed for embedded systems
- Clean and simple register-level API

## Dependencies

- ESP-IDF (ESP32-S3 SDK)
- HAL for I2C communication (`platform_esp32s3.h`)


## Testing Color Sensing via UART

To test the APDS9960 color sensing functionality, you can use the provided `main.c` firmware for the ESP32-S3 and the `test.py` script on your computer. The ESP32-S3 will send the color sensor data (Red, Green, Blue, Clear) over UART, and `test.py` will read and display this data on the host PC.

### UART Test Workflow

1. Flash the ESP32-S3 with the `main.c` firmware.
2. Connect the ESP32-S3 to your PC via USB/UART.
3. Run `test.py` to receive and display the color data in real-time.