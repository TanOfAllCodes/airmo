#include "solar.h"
#include "modules/i2c/i2c.h"

void Solar::init() {
    // Configure integration time, gain
}

bool Solar::read(SolarData &out) {
    (void)out;
    // Read ALS / UVS channels, compute irradiance + UV index
    return false; // TODO
}
