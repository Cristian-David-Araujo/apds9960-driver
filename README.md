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


