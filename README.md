# AIRMO

Bare-metal FreeRTOS firmware for atmospheric monitoring on STM32 microcontrollers. No HAL, no vendor SDK — all peripheral access via direct register writes.

---

## Table of Contents

1. [Cloning the Repository](#1-cloning-the-repository)
2. [Installing Dependencies](#2-installing-dependencies)
3. [Installing FreeRTOS Kernel](#3-installing-freertos-kernel)
4. [Repository Structure](#4-repository-structure)
5. [Makefile Reference](#5-makefile-reference)
   - [Target Selection](#target-selection)
   - [Build](#build)
   - [Flash](#flash)
   - [Utilities](#utilities)
   - [Conditional Features](#conditional-features)
6. [Project Documentation](#6-project-documentation)

---

## 1. Cloning the Repository

```bash
git clone https://github.com/TanOfAllCodes/airmo.git
cd airmo
```

---

## 2. Installing Dependencies

The only required toolchain is the ARM GCC cross-compiler and the ST-Link flash utility.

**On Ubuntu / Debian:**

```bash
sudo apt update
sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi stlink-tools
```

**On Arch Linux:**

```bash
sudo pacman -S arm-none-eabi-gcc arm-none-eabi-binutils stlink
```

**On macOS (Homebrew):**

```bash
brew install arm-none-eabi-gcc stlink
```

Verify the toolchain is available:

```bash
arm-none-eabi-g++ --version
st-flash --version
```

---

## 3. Installing FreeRTOS Kernel

FreeRTOS is **not included** in the repository (it is listed in `.gitignore`). A setup script is provided to clone it:

```bash
cd rstm32
bash setup_freertos.sh
```

This clones [`FreeRTOS/FreeRTOS-Kernel`](https://github.com/FreeRTOS/FreeRTOS-Kernel) into `rstm32/FreeRTOS-Kernel/` at shallow depth. The script is idempotent — running it again when the directory already exists is safe.

---

## 4. Repository Structure

```
airmo/
├── README.md                      ← This file
├── TECHNICAL.md                   ← Hardware, sensors, and system design
├── PROJECT.md                     ← Project Overview
├── arch.pdf                       ← System architecture diagram
├── bom.xlsx                       ← Bill of materials
└── rstm32/                        ← Firmware project root
    ├── Makefile                   ← Multi-target build system
    ├── setup_freertos.sh          ← FreeRTOS Kernel clone script
    ├── FreeRTOS-Kernel/           ← Vendored FreeRTOS (cloned by setup script)
    ├── targets/                   ← Board-specific files
    │   ├── STM32F07X/
    │   │   ├── target.h           ← RCC + GPIOC register map
    │   │   ├── stm32f07x.ld       ← Linker script (128K flash / 16K RAM)
    │   │   └── FreeRTOSConfig.h   ← 8 MHz, 12KB heap, Cortex-M0 NVIC
    │   └── STM32H753/
    │       ├── target.h           ← RCC + GPIOB + ETH register map
    │       ├── stm32h753.ld       ← Linker script (2MB flash / 1MB RAM)
    │       └── FreeRTOSConfig.h   ← 400 MHz, 64KB heap, FPU, Cortex-M7 NVIC
    └── src/
        ├── main.cpp               ← Reset handler, vector table, task creation
        ├── modules/               ← Hardware drivers (one class per peripheral)
        │   ├── blink4s/           ← GPIO driver for onboard LEDs (F07X)
        │   ├── i2c/               ← Shared I2C bus driver
        │   ├── gnss/              ← GNSS receiver (u-blox, I2C 0x42)
        │   ├── imu/               ← IMU — accel + gyro (LSM6DSO, I2C 0x6A)
        │   ├── thermal_humidity/  ← Temp + humidity (SHT4x, I2C 0x44)
        │   ├── barometer/         ← Pressure + temp (BMP3xx, I2C 0x76)
        │   ├── aerosol/           ← Particulate matter (SPS30, I2C 0x69)
        │   ├── solar/             ← Solar irradiance + UV (VEML6075, I2C 0x10)
        │   ├── infrared/          ← IR thermometer (MLX90614, I2C 0x5A)
        │   └── ethernet/          ← ETH MAC + LAN8742A PHY driver (H753 only)
        └── services/              ← Application-level FreeRTOS tasks
            ├── ledring/           ← LED animation service (F07X only)
            ├── sensors/           ← Polls all 7 sensors at 1 Hz
            └── telemetry/         ← TCP telemetry at 100 ms (H753 only)
```

---

## 5. Makefile Reference

All commands are run from inside the `rstm32/` directory:

```bash
cd rstm32
```

### Target Selection

The `TARGET` variable selects the board. The default is `STM32F07X`.

| `TARGET` | Board | Core | Flash / RAM |
|---|---|---|---|
| `STM32F07X` | STM32F072 Discovery | Cortex-M0 | 128 KB / 16 KB |
| `STM32H753` | STM32H753ZI Nucleo-144 | Cortex-M7 | 2 MB / 1 MB |

Pass it on the command line to override the default:

```bash
make TARGET=STM32F07X    # explicit (same as default)
make TARGET=STM32H753    # Nucleo-144
```

### Build

```bash
make                        # Build for STM32F07X (default)
make TARGET=STM32H753       # Build for STM32H753
```

Compiled objects and the final ELF/BIN are written to `build/<TARGET>/`. Dependency files (`.d`) are generated automatically for incremental rebuilds.

### Flash

The `flash` target erases the device, writes the binary, and resets the MCU so it starts immediately:

```bash
make flash                        # Build + erase + flash STM32F07X
make flash TARGET=STM32H753       # Build + erase + flash STM32H753
```

Requires `st-flash` and an ST-Link probe connected to the board. The flash address is `0x08000000` for both targets.

### Utilities

```bash
make clean                  # Delete the entire build/ directory
make clear                  # Erase flash only (no build)
make info                   # Print current target configuration
```

`make info` output example (for F07X):

```
TARGET          = STM32F07X
CPU_FLAGS       = -mcpu=cortex-m0 -mthumb
FREERTOS_PORT   = GCC/ARM_CM0
LDSCRIPT        = targets/STM32F07X/stm32f07x.ld
BUILD_DIR       = build/STM32F07X
FLASH_ADDR      = 0x08000000
HAS_ETHERNET    = 0
```

### Conditional Features

The build system enables features based on the selected target:

| Feature | Macro | Enabled for |
|---|---|---|
| LED ring animation | `HAS_RING=1` | `STM32F07X` |
| Ethernet + TCP telemetry | `HAS_ETHERNET=1` | `STM32H753` |

These are passed as preprocessor defines and gate both source file inclusion (in the Makefile) and code paths (via `#if HAS_RING` / `#if HAS_ETHERNET` in `main.cpp`).

---

## 6. Project Documentation

| Document | Description |
|---|---|
| [`PROJECT.md`](PROJECT.md) | Hardware overview, microcontroller specs, sensor datasheets, FreeRTOS task architecture, telemetry protocol |
| [`arch.pdf`](arch.pdf) | System architecture diagram |
| [`bom.xlsx`](bom.xlsx) | Bill of materials |
