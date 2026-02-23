#ifndef SOLAR_H
#define SOLAR_H

#include <stdint.h>

struct SolarData {
    uint32_t irradiance_mw_m2;  // mW/m² (visible + IR combined or channel)
    uint16_t uv_index_c100;     // UV index × 100
};

class Solar {
public:
    static void init();
    static bool read(SolarData &out);

    static constexpr uint8_t I2C_ADDR = 0x10; // typical VEML6075 / LTR-390
};

#endif // SOLAR_H
