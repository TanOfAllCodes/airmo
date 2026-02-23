#include "telemetry.h"
#include "modules/ethernet/ethernet.h"
#include "services/sensors/sensors.h"

// ── Static storage ──────────────────────────────────────────────────────────
TickType_t Telemetry::sendInterval = pdMS_TO_TICKS(100);  // 10 Hz
uint16_t   Telemetry::seq_         = 0;

// ── Default network config (static IP, point-to-point) ─────────────────────
static const EthConfig defaultCfg = {
    .mac         = { 0x02, 0x00, 0x00, 0x00, 0x00, 0x01 },  // locally-administered
    .ip          = { 192, 168, 1, 10 },
    .gateway     = { 192, 168, 1, 1 },
    .netmask     = { 255, 255, 255, 0 },
    .server_ip   = { 192, 168, 1, 100 },   // Linux laptop
    .server_port = 5000
};

// ── Init ────────────────────────────────────────────────────────────────────
void Telemetry::init() {
    Ethernet::init(defaultCfg);

    // Wait for link and establish connection
    // Retry loop — tolerates cable not yet plugged in at boot
    while (!Ethernet::isConnected()) {
        Ethernet::poll();

        if (Ethernet::state() == EthState::LINK_UP) {
            Ethernet::connect();
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// ── Send loop (runs inside a FreeRTOS task) ─────────────────────────────────
void Telemetry::start() {
    TelemetryPacket pkt;

    while (1) {
        // Keep connection alive / reconnect
        Ethernet::poll();

        if (!Ethernet::isConnected()) {
            if (Ethernet::state() == EthState::LINK_UP) {
                Ethernet::connect();
            }
            vTaskDelay(pdMS_TO_TICKS(500));
            continue;
        }

        // Build and send
        buildPacket(pkt);
        Ethernet::send(reinterpret_cast<const uint8_t *>(&pkt),
                        sizeof(pkt));

        vTaskDelay(sendInterval);
    }
}

// ── Packet assembly ─────────────────────────────────────────────────────────
void Telemetry::buildPacket(TelemetryPacket &pkt) {
    pkt.sync[0]       = 0xAA;
    pkt.sync[1]       = 0x55;
    pkt.seq            = seq_++;
    pkt.timestamp_ms   = xTaskGetTickCount();  // ms (if tick = 1kHz)

    // GNSS
    pkt.gps_lat        = Sensors::gnss.latitude;
    pkt.gps_lon        = Sensors::gnss.longitude;
    pkt.gps_alt_mm     = Sensors::gnss.altitude_mm;
    pkt.gps_sats       = Sensors::gnss.satellites;
    pkt.gps_fix        = Sensors::gnss.fix;

    // IMU
    pkt.accel_x        = Sensors::imu.accel_x;
    pkt.accel_y        = Sensors::imu.accel_y;
    pkt.accel_z        = Sensors::imu.accel_z;
    pkt.gyro_x         = Sensors::imu.gyro_x;
    pkt.gyro_y         = Sensors::imu.gyro_y;
    pkt.gyro_z         = Sensors::imu.gyro_z;

    // Thermal + Humidity
    pkt.temp_c100      = Sensors::thermalHumidity.temperature_c100;
    pkt.hum_c100       = Sensors::thermalHumidity.humidity_c100;

    // Barometer
    pkt.pressure_pa    = Sensors::barometer.pressure_pa;
    pkt.baro_temp_c100 = Sensors::barometer.temperature_c100;

    // Aerosol
    pkt.pm1_0          = Sensors::aerosol.pm1_0;
    pkt.pm2_5          = Sensors::aerosol.pm2_5;
    pkt.pm10           = Sensors::aerosol.pm10;

    // Solar
    pkt.irradiance_mw  = Sensors::solar.irradiance_mw_m2;
    pkt.uv_index_c100  = Sensors::solar.uv_index_c100;

    // Infrared
    pkt.ir_obj_c100    = Sensors::infrared.object_temp_c100;
    pkt.ir_amb_c100    = Sensors::infrared.ambient_temp_c100;

    // CRC over everything except the crc field itself
    pkt.crc = crc16(reinterpret_cast<const uint8_t *>(&pkt),
                    sizeof(pkt) - sizeof(pkt.crc));
}

// ── CRC-16/CCITT-FALSE ──────────────────────────────────────────────────────
uint16_t Telemetry::crc16(const uint8_t *data, uint16_t len) {
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (uint8_t bit = 0; bit < 8; bit++) {
            crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : crc << 1;
        }
    }
    return crc;
}
