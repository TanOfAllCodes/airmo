#ifndef IMU_H
#define IMU_H

#include <stdint.h>

struct ImuData {
    int16_t accel_x;    // mg
    int16_t accel_y;
    int16_t accel_z;
    int16_t gyro_x;     // mdps
    int16_t gyro_y;
    int16_t gyro_z;
};

class Imu {
public:
    static void init();
    static bool read(ImuData &out);

    static constexpr uint8_t I2C_ADDR = 0x6A; // typical LSM6DSO / ICM-series
};

#endif // IMU_H
