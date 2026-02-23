#include "aerosol.h"
#include "modules/i2c/i2c.h"

void Aerosol::init() {
    // Start measurement, set cleaning interval
}

bool Aerosol::read(AerosolData &out) {
    (void)out;
    // Read PM1.0, PM2.5, PM10 mass concentration registers
    return false; // TODO
}
