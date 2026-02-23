#ifndef TELEMETRY_H
#define TELEMETRY_H

#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>

/*
 * Telemetry service — sends sensor readings to a TCP server at 10 Hz.
 *
 * Runs as a FreeRTOS task. On boards without ethernet (HAS_ETHERNET=0)
 * this file is not compiled at all (excluded from the Makefile).
 *
 * Frame format (packed, little-endian):
 *   [ sync 0xAA 0x55 ] [ seq_u16 ] [ timestamp_ms_u32 ]
 *   [ gnss      ... ]
 *   [ imu       ... ]
 *   [ therm/hum ... ]
 *   [ baro      ... ]
 *   [ aerosol   ... ]
 *   [ solar     ... ]
 *   [ infrared  ... ]
 *   [ crc16          ]
 */

// ── Telemetry packet ────────────────────────────────────────────────────────

struct __attribute__((packed)) TelemetryPacket {
    // Header
    uint8_t  sync[2];        // { 0xAA, 0x55 }
    uint16_t seq;
    uint32_t timestamp_ms;

    // GNSS
    int32_t  gps_lat;        // deg × 1e7
    int32_t  gps_lon;
    int32_t  gps_alt_mm;
    uint8_t  gps_sats;
    uint8_t  gps_fix;

    // IMU
    int16_t  accel_x;        // mg
    int16_t  accel_y;
    int16_t  accel_z;
    int16_t  gyro_x;         // mdps
    int16_t  gyro_y;
    int16_t  gyro_z;

    // Thermal + Humidity
    int16_t  temp_c100;      // °C × 100
    uint16_t hum_c100;       // %RH × 100

    // Barometer
    uint32_t pressure_pa;
    int16_t  baro_temp_c100;

    // Aerosol
    uint16_t pm1_0;          // µg/m³
    uint16_t pm2_5;
    uint16_t pm10;

    // Solar irradiation
    uint32_t irradiance_mw;  // mW/m²
    uint16_t uv_index_c100;

    // Infrared
    int16_t  ir_obj_c100;    // °C × 100
    int16_t  ir_amb_c100;

    // Footer
    uint16_t crc;
};

// ── Service API ─────────────────────────────────────────────────────────────

class Telemetry {
public:
    static void init();   // Ethernet init + TCP connect
    static void start();  // blocking send loop (call from FreeRTOS task)

    static TickType_t sendInterval;   // default 100ms (10 Hz)

private:
    static uint16_t seq_;
    static void     buildPacket(TelemetryPacket &pkt);
    static uint16_t crc16(const uint8_t *data, uint16_t len);
};

#endif // TELEMETRY_H
