[English](README.md) | [简体中文](README_CN.md)

# Project Title

# ESP32-Based Electronic Desktop Pet

> An ESP32-S3 electronic desktop pet built with ESP-IDF. It combines an SSD1306 OLED expression engine, a small motor-control interface, Wi-Fi provisioning, and an event/state-driven firmware structure.

[![Platform](https://img.shields.io/badge/platform-ESP32--S3-E7352C?logo=espressif&logoColor=white)](https://www.espressif.com/en/products/socs/esp32-s3)
[![Framework](https://img.shields.io/badge/framework-ESP--IDF%206.0.1-00979D)](https://docs.espressif.com/projects/esp-idf/)
[![Language](https://img.shields.io/badge/language-C%2B%2B-00599C?logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![License](https://img.shields.io/badge/license-not%20specified-lightgrey)](#license)

## Project status

**This is the active project repository and the single source of truth for the current firmware.**

The similarly named [graduation-project repository](https://github.com/Midst-exe/Design-and-Implementation-of-an-ESP32-Based-Electronic-Desktop-Pet) is an earlier/archival project space. Please use this repository for the current ESP-IDF implementation.

> Hardware-dependent functionality has not yet been published with a reproducible test record or release binary. Build and validate it against your own board before relying on it.

## What is implemented

- **ESP32-S3 firmware foundation** using ESP-IDF, with the build target locked in `dependencies.lock`.
- **SSD1306 OLED support** with framebuffer, graphics, driver, font, and bitmap-resource modules.
- **Expression playback** for bitmap-based facial animations.
- **Event and state management** to coordinate system state, including Wi-Fi configuration status.
- **Wi-Fi provisioning flow** and a small web-server component for configuration-status interaction.
- **Application tasks** for Wi-Fi initialization, expression updates, UART transmission, and test hooks.
- **Board-support layer** for I2C, UART, GPIO, and non-volatile storage initialization.
- An **Arduino sketch prototype** (`main/main.ino`) for I2S microphone capture, WebSocket audio streaming, OLED expressions, and motor updates. It is separate from the ESP-IDF entry point and should be treated as an experimental reference rather than the default build target.

## Architecture

```text
ESP-IDF app_main
  ├─ Board support: GPIO / I2C / UART / NVS
  ├─ Event manager + state manager
  ├─ Wi-Fi initialization and provisioning
  ├─ OLED stack
  │   ├─ SSD1306 driver
  │   ├─ framebuffer + graphics
  │   └─ expression assets and playback
  └─ Application tasks
      ├─ Wi-Fi task
      ├─ expression task
      ├─ UART task
      └─ test task
```

## Repository layout

| Path | Purpose |
| --- | --- |
| [`main/main.cpp`](main/main.cpp) | ESP-IDF application entry point |
| [`components/Oled`](components/Oled) | SSD1306 driver, framebuffer, and graphics layer |
| [`components/Expressions`](components/Expressions) | Expression animation logic |
| [`components/Oled_resource`](components/Oled_resource) | Expression bitmaps and fonts |
| [`components/State_Manager`](components/State_Manager) | Shared system and Wi-Fi configuration state |
| [`components/Event_Manager`](components/Event_Manager) | Event definitions and dispatch |
| [`components/WebServer`](components/WebServer) | Provisioning-related web-server component |
| [`components/App_tasks`](components/App_tasks) | Application-level FreeRTOS tasks |
| [`components/board`](components/board) | Board support and peripheral initialization |
| [`main/main.ino`](main/main.ino) | Separate Arduino prototype |

## Hardware

The current ESP-IDF entry point documents the following hardware interfaces:

| Peripheral | Interface | ESP32-S3 pins |
| --- | --- | --- |
| OLED display | I2C | SCL GPIO16, SDA GPIO15 |
| I2S microphone | I2S | BCLK GPIO11, WS GPIO12, data GPIO8 |
| I2S amplifier | I2S | BCLK GPIO11, LRC GPIO12, data GPIO10 |
| Motor driver | GPIO | IN1 GPIO4, IN2 GPIO5, IN3 GPIO6, IN4 GPIO7 |

Expected modules include an ESP32-S3 board, an SSD1306 0.96-inch OLED, an INMP441 I2S microphone, an I2S amplifier such as MAX98357A, and a four-input motor driver. Confirm voltage levels, pin assignments, and shared I2S wiring against your own schematic before powering the board.

## Getting started

### Prerequisites

- ESP-IDF **v6.0.1** (the version recorded by the dependency lock file)
- An ESP32-S3 development board
- A serial connection to the board

### Build and flash

```bash
git clone https://github.com/Midst-exe/ESP32-Based-Electronic-Desktop-Pet.git
cd ESP32-Based-Electronic-Desktop-Pet

idf.py set-target esp32s3
idf.py build
idf.py -p <PORT> flash monitor
```

Replace `<PORT>` with your board's serial port (for example, `COM3` on Windows or `/dev/ttyACM0` on Linux).

### First validation checklist

1. Build successfully with ESP-IDF.
2. Flash the firmware and open the serial monitor.
3. Verify I2C/OLED initialization and basic expression rendering.
4. Exercise the Wi-Fi provisioning path and confirm its reported state.
5. Test motor control with the actuator safely unloaded.

## Notes for contributors

- Keep secrets out of source control. Wi-Fi credentials and local server addresses belong in a local configuration mechanism, not committed firmware.
- The Arduino sketch and ESP-IDF application use different entry points and pin maps. Do not mix them without reconciling the hardware configuration.
- When adding a hardware feature, document its wiring, configuration, and a reproducible validation result in this README or `docs/`.

## Roadmap

- [ ] Add wiring diagram and board photo
- [ ] Publish a short demonstration video or GIF
- [ ] Add a reproducible hardware-validation checklist and serial-log evidence
- [ ] Consolidate the Arduino prototype into, or clearly separate it from, the ESP-IDF implementation
- [ ] Add a license and release tags

## License

No license has been specified yet. Add a `LICENSE` file before allowing reuse beyond evaluation or personal learning.
