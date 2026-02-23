# AIRMO — Project Documentation

Detailed hardware, sensor, and firmware architecture reference for the AIRMO atmospheric monitoring system.

See [`README.md`](README.md) for build and toolchain instructions.

---

## Table of Contents

1. [System Overview](#1-system-overview)
2. [Microcontrollers](#2-microcontrollers)
3. [Sensors](#3-sensors)
4. [FreeRTOS Task Architecture](#4-freertos-task-architecture)
5. [Telemetry Protocol](#5-telemetry-protocol)
6. [Memory Layout](#6-memory-layout)

---

## 1. System Overview

AIRMO is a multi-node atmospheric monitoring system. Each node reads environmental data from a set of I2C sensors and streams a packed telemetry packet over TCP/Ethernet to a base station.

- **Edge node** — STM32F072 Discovery. Runs sensor polling and LED status indication. No network connectivity.
- **Gateway node** — STM32H753ZI Nucleo-144. Runs sensor polling and transmits telemetry at 100 ms intervals over Ethernet (LAN8742A PHY, RMII).

---

## 2. Microcontrollers

### STM32F072 Discovery (`TARGET=STM32F07X`)

| Property | Value |
|---|---|
| Core | ARM Cortex-M0 |
| Max Clock | 48 MHz (running at 8 MHz HSI, no PLL configured) |
| Flash | 128 KB at `0x08000000` |
| SRAM | 16 KB at `0x20000000` |
| FreeRTOS port | `GCC/ARM_CM0` |
| FreeRTOS heap | 12 KB (`heap_4`) |
| NVIC priority bits | 2 |
| Onboard LEDs | LD3 (Red) PC6 · LD6 (Blue) PC7 · LD4 (Orange) PC8 · LD5 (Green) PC9 |

### STM32H753ZI Nucleo-144 (`TARGET=STM32H753`)

| Property | Value |
|---|---|
| Core | ARM Cortex-M7 (FPU, double-precision) |
| Max Clock | 480 MHz (configured at 400 MHz) |
| Flash | 2 MB at `0x08000000` |
| SRAM | 1 MB (DTCM + AXI SRAM) |
| FreeRTOS port | `GCC/ARM_CM7/r0p1` |
| FreeRTOS heap | 64 KB (`heap_4`) |
| NVIC priority bits | 4 |
| Ethernet PHY | LAN8742A (RMII, onboard) |
| Onboard LEDs | LD1 (Green) PB0 · LD2 (Blue) PB7 · LD3 (Red) PB14 |

---

## 3. Sensors

All sensors communicate over a shared I2C bus. Drivers live in `rstm32/src/modules/`.

| Module | Sensor | Measurement | I2C Address | Data struct |
|---|---|---|---|---|
| `gnss/` | u-blox (any, default addr) | Latitude, longitude, altitude, fix quality, satellite count | `0x42` | `GnssData` |
| `imu/` | LSM6DSO (or ICM-series) | 3-axis accelerometer (mg), 3-axis gyroscope (mdps) | `0x6A` | `ImuData` |
| `thermal_humidity/` | SHT4x (or HDC series) | Temperature (°C × 100), relative humidity (%RH × 100) | `0x44` | `ThermalHumidityData` |
| `barometer/` | BMP3xx (or LPS22HH) | Pressure (Pa), temperature (°C × 100) | `0x76` | `BarometerData` |
| `aerosol/` | SPS30 (or PMSA003I) | PM1.0, PM2.5, PM10 (µg/m³) | `0x69` | `AerosolData` |
| `solar/` | VEML6075 (or LTR-390) | Solar irradiance (mW/m²), UV index (× 100) | `0x10` | `SolarData` |
| `infrared/` | MLX90614 | Object temperature (°C × 100), ambient temperature (°C × 100) | `0x5A` | `InfraredData` |

All driver `read()` methods return `bool` — `true` on successful read, `false` on I2C error or sensor not ready. The `sensors` service polls all 7 at 1 Hz and stores results in module-static buffers.

---

## 4. FreeRTOS Task Architecture

### STM32F07X

```
Reset_Handler
└─ main_app()
     ├─ xTaskCreate(vLedRingTask,  stack=256w, priority=1)   [HAS_RING]
     ├─ xTaskCreate(vSensorsTask,  stack=512w, priority=1)
     └─ vTaskStartScheduler()
          ├─ vLedRingTask
          │    ├─ LedRing::init()    → 11× flash all LEDs at 50 ms
          │    └─ LedRing::start()   → clockwise / anticlockwise LED sweep at 100 ms (infinite)
          └─ vSensorsTask
               └─ Sensors::start()  → poll all 7 sensors, vTaskDelay(1000 ms) (infinite)
```

### STM32H753

```
Reset_Handler
└─ main_app()
     ├─ xTaskCreate(vSensorsTask,   stack=512w,  priority=1)
     ├─ xTaskCreate(vTelemetryTask, stack=1024w, priority=3)  [HAS_ETHERNET]
     └─ vTaskStartScheduler()
          ├─ vSensorsTask
          │    └─ Sensors::start()  → poll all 7 sensors, vTaskDelay(1000 ms) (infinite)
          └─ vTelemetryTask
               └─ Telemetry::start() → build TelemetryPacket, send via ETH, vTaskDelay(100 ms)
```

### FreeRTOS Configuration

| Parameter | STM32F07X | STM32H753 |
|---|---|---|
| `configCPU_CLOCK_HZ` | 8,000,000 | 400,000,000 |
| `configTICK_RATE_HZ` | 1000 | 1000 |
| `configTOTAL_HEAP_SIZE` | 12 × 1024 | 64 × 1024 |
| `configMINIMAL_STACK_SIZE` | 128 words | 128 words |
| `configUSE_MALLOC_FAILED_HOOK` | 1 | 1 |
| `configCHECK_FOR_STACK_OVERFLOW` | 2 | 2 |
| Heap scheme | `heap_4` | `heap_4` |

---

## 5. Telemetry Protocol

The telemetry service (`rstm32/src/services/telemetry/`) runs on the H753 gateway only (`HAS_ETHERNET=1`). It sends a packed binary struct over TCP at 100 ms intervals.

**Static network configuration:**

| Node | IP |
|---|---|
| STM32H753 | `192.168.1.10` |
| Base station | `192.168.1.100:5000` |

**Packet format (`TelemetryPacket`, packed, little-endian):**

| Field | Type | Source |
|---|---|---|
| `timestamp_ms` | `uint32_t` | `xTaskGetTickCount()` |
| `latitude` | `int32_t` | `GnssData::latitude` (deg × 1e7) |
| `longitude` | `int32_t` | `GnssData::longitude` (deg × 1e7) |
| `altitude_mm` | `int32_t` | `GnssData::altitude_mm` |
| `gnss_fix` | `uint8_t` | `GnssData::fix` |
| `gnss_sats` | `uint8_t` | `GnssData::satellites` |
| `accel_x/y/z` | `int16_t` × 3 | `ImuData` (mg) |
| `gyro_x/y/z` | `int16_t` × 3 | `ImuData` (mdps) |
| `temperature_c100` | `int16_t` | `ThermalHumidityData` (°C × 100) |
| `humidity_c100` | `uint16_t` | `ThermalHumidityData` (%RH × 100) |
| `pressure_pa` | `uint32_t` | `BarometerData` (Pa) |
| `baro_temp_c100` | `int16_t` | `BarometerData` (°C × 100) |
| `pm1_0/pm2_5/pm10` | `uint16_t` × 3 | `AerosolData` (µg/m³) |
| `irradiance_mw_m2` | `uint32_t` | `SolarData` (mW/m²) |
| `uv_index_c100` | `uint16_t` | `SolarData` (UV index × 100) |
| `obj_temp_c100` | `int16_t` | `InfraredData` (°C × 100) |
| `amb_temp_c100` | `int16_t` | `InfraredData` (°C × 100) |
| `crc16` | `uint16_t` | CRC-16/CCITT over all preceding bytes |

---

## 6. Memory Layout

### STM32F07X (16 KB RAM)

| Region | Size | Usage |
|---|---|---|
| `.data` | ~16 B | Initialized globals |
| `.bss` | ~12.7 KB | FreeRTOS heap (`ucHeap`, 12 KB) + uninit globals |
| MSP stack | ~3.6 KB | Available at reset before scheduler starts |

### STM32H753 (1 MB RAM — DTCM + AXI SRAM)

| Region | Address | Size |
|---|---|---|
| DTCM RAM | `0x20000000` | 128 KB — stack + `.data` + `.bss` |
| AXI SRAM | `0x24000000` | 512 KB — available for large buffers |
| SRAM1/2/3 | `0x30000000` | 288 KB |
| SRAM4 | `0x38000000` | 64 KB |
