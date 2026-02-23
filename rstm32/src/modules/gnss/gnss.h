#ifndef GNSS_H
#define GNSS_H

#include <stdint.h>

struct GnssData {
    int32_t  latitude;      // degrees × 1e7
    int32_t  longitude;     // degrees × 1e7
    int32_t  altitude_mm;   // mm above MSL
    uint8_t  satellites;
    uint8_t  fix;           // 0=none, 1=2D, 2=3D
};

class Gnss {
public:
    static void init();
    static bool read(GnssData &out);

    static constexpr uint8_t I2C_ADDR = 0x42; // u-blox default
};

#endif // GNSS_H
