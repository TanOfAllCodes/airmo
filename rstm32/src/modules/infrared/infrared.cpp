#include "infrared.h"
#include "modules/i2c/i2c.h"

void Infrared::init() {
    // Configure emissivity, measurement mode if needed
}

bool Infrared::read(InfraredData &out) {
    (void)out;
    // Read Tobj and Tambient registers, apply scaling
    return false; // TODO
}
