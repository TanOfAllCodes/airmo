#include "imu.h"
#include "modules/i2c/i2c.h"

void Imu::init() {
    // Set ODR, full-scale range, enable accel + gyro
}

bool Imu::read(ImuData &out) {
    (void)out;
    // Burst-read accel + gyro registers
    return false; // TODO
}
