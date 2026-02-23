# 📋 AIRMO  — Repository Summary

## 🎯 Overview
Multi-target bare-metal **FreeRTOS firmware** for **STM32** microcontrollers, written in C++11. No HAL, no vendor SDK — all peripheral access is via direct register manipulation.

---

## 🏗️ Architecture

```
airmo/
└── rstm32/                       # Main firmware project
    ├── src/
    │   ├── main.cpp               ⭐ Entry point, vector table, FreeRTOS task creation
    │   ├── modules/               🧩 Reusable hardware drivers
    │   │   ├── blink4s/           → LED GPIO driver
    │   │   └── uartread/          → UART input module (stub)
    │   └── services/              ⚙️  Application-level logic
    │       └── ring/              → LED ring animation service
    ├── targets/                   🎯 Board-specific files
    │   ├── STM32F07X/             → Discovery F072 (Cortex-M0, 128K/16K)
    │   │   ├── target.h           → RCC & GPIOC register map
    │   │   ├── stm32f07x.ld      → Linker script
    │   │   └── FreeRTOSConfig.h   → 8MHz, 8KB heap, 2-bit NVIC
    │   └── STM32H753/             → Nucleo-144 H753ZI (Cortex-M7, 2MB/1MB)
    │       ├── target.h           → RCC & GPIOB register map
    │       ├── stm32h753.ld       → Linker script (DTCM + AXI SRAM)
    │       └── FreeRTOSConfig.h   → 400MHz, 64KB heap, 4-bit NVIC, FPU
    ├── FreeRTOS-Kernel/           📦 Vendored FreeRTOS source (multi-port)
    └── Makefile                   ⭐ TARGET-selectable build system
```

---

## ⭐ Key Files

| File | Role |
|---|---|
| `Makefile` | `make TARGET=STM32F07X` or `make TARGET=STM32H753` — selects CPU, port, linker script |
| `src/main.cpp` | Reset handler, vector table, `vRingTask` creation, scheduler start |
| `targets/*/target.h` | Per-board register definitions (RCC, GPIO) |
| `targets/*/FreeRTOSConfig.h` | Per-board RTOS tuning (clock, heap, NVIC priority bits) |
| `targets/*/*.ld` | Per-board memory layout |
| `src/services/ring/ring.cpp` | Boot flash sequence + continuous LED toggle loop |
| `src/modules/blink4s/blink4s.cpp` | GPIO driver for onboard LEDs |

---

## 🔄 Task Architecture

```
Reset_Handler
  └─ main_app()
       ├─ xTaskCreate(vRingTask)   [stack: 256 words, priority: 1]
       └─ vTaskStartScheduler()
            └─ vRingTask
                 ├─ Ring::init()   → 10× blink all LEDs @ 100ms
                 └─ Ring::start()  → toggle all LEDs @ 500ms (infinite)
```

The **service layer** (`ring`) orchestrates behavior; the **module layer** (`blink4s`) owns hardware.

---

## 🛠️ Build & Flash

```bash
make                        # Default: TARGET=STM32F07X
make TARGET=STM32H753       # Build for Nucleo H753ZI
make flash TARGET=STM32H753 # Erase + flash
make clean                  # Remove all build artifacts
make info                   # Print current target config
```

**Toolchain:** `arm-none-eabi-g++` · **Heap scheme:** `heap_4`

| Target | Core | FreeRTOS Port | Clock | Flash / RAM |
|---|---|---|---|---|
| `STM32F07X` | Cortex-M0 | `GCC/ARM_CM0` | 8 MHz | 128K / 16K |
| `STM32H753` | Cortex-M7 | `GCC/ARM_CM7/r0p1` | 400 MHz | 2MB / 1MB |

---

## 📌 Notable Design Choices

- 🚫 **No vendor HAL** — registers accessed directly via per-target `target.h`
- 🎯 **Multi-target** — `make TARGET=…` selects board, CPU flags, FreeRTOS port, and linker script
- 🗂️ **Modules vs Services** — hardware drivers (`modules/`) are decoupled from application logic (`services/`)
- ⚡ **Minimal footprint** — `nano.specs`, no exceptions/RTTI, `-O2`
- 🔴 **Error indicator** — scheduler failure lights an LED as a hardware fault signal
