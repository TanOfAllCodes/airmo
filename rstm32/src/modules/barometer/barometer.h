#ifndef BAROMETER_H
#define BAROMETER_H

#include <stdint.h>

struct BarometerData {
    uint32_t pressure_pa;       // Pa
    int16_t  temperature_c100;  // °C × 100
};

class Barometer {
public:
    static void init();
    static bool read(BarometerData &out);

    static constexpr uint8_t I2C_ADDR = 0x76; // typical BMP3xx / LPS22HH
};

#endif // BAROMETER_H
