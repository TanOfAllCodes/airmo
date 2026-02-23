#include "barometer.h"
#include "modules/i2c/i2c.h"

void Barometer::init() {
    // Configure oversampling, ODR, IIR filter
}

bool Barometer::read(BarometerData &out) {
    (void)out;
    // Read pressure + temperature compensation registers
    return false; // TODO
}
