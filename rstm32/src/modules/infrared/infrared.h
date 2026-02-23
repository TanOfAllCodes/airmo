#ifndef INFRARED_H
#define INFRARED_H

#include <stdint.h>

struct InfraredData {
    int16_t  object_temp_c100;  // °C × 100 (non-contact surface temp)
    int16_t  ambient_temp_c100; // °C × 100
};

class Infrared {
public:
    static void init();
    static bool read(InfraredData &out);

    static constexpr uint8_t I2C_ADDR = 0x5A; // typical MLX90614
};

#endif // INFRARED_H
