#include "sensors.h"
#include "modules/i2c/i2c.h"

// ── Static storage ──────────────────────────────────────────────────────────
GnssData             Sensors::gnss             = {};
ImuData              Sensors::imu              = {};
ThermalHumidityData  Sensors::thermalHumidity  = {};
BarometerData        Sensors::barometer        = {};
AerosolData          Sensors::aerosol          = {};
SolarData            Sensors::solar            = {};
InfraredData         Sensors::infrared         = {};

TickType_t Sensors::pollInterval = pdMS_TO_TICKS(1000);

// ── Init ────────────────────────────────────────────────────────────────────
void Sensors::init() {
    // Bus first, then every device on it
    I2C::init();

    Gnss::init();
    Imu::init();
    ThermalHumidity::init();
    Barometer::init();
    Aerosol::init();
    Solar::init();
    Infrared::init();
}

// ── Poll loop (runs inside a FreeRTOS task) ─────────────────────────────────
void Sensors::start() {
    while (1) {
        Gnss::read(gnss);
        Imu::read(imu);
        ThermalHumidity::read(thermalHumidity);
        Barometer::read(barometer);
        Aerosol::read(aerosol);
        Solar::read(solar);
        Infrared::read(infrared);

        vTaskDelay(pollInterval);
    }
}
