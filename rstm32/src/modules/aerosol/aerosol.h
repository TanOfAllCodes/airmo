#ifndef AEROSOL_H
#define AEROSOL_H

#include <stdint.h>

struct AerosolData {
    uint16_t pm1_0;     // µg/m³
    uint16_t pm2_5;
    uint16_t pm10;
};

class Aerosol {
public:
    static void init();
    static bool read(AerosolData &out);

    static constexpr uint8_t I2C_ADDR = 0x69; // typical SPS30 / PMSA003I
};

#endif // AEROSOL_H
