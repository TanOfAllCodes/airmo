#ifndef THERMAL_HUMIDITY_H
#define THERMAL_HUMIDITY_H

#include <stdint.h>

struct ThermalHumidityData {
    int16_t  temperature_c100;  // °C × 100
    uint16_t humidity_c100;     // %RH × 100
};

class ThermalHumidity {
public:
    static void init();
    static bool read(ThermalHumidityData &out);

    static constexpr uint8_t I2C_ADDR = 0x44; // typical SHT4x / HDC series
};

#endif // THERMAL_HUMIDITY_H
