#ifndef I2C_H
#define I2C_H

#include <stdint.h>

/*
 * Minimal I2C bus driver.
 * Provides blocking read/write for all sensor modules on the shared bus.
 */

class I2C {
public:
    static void init();

    // Write `len` bytes from `data` to `regAddr` on device `devAddr` (7-bit).
    static bool write(uint8_t devAddr, uint8_t regAddr,
                      const uint8_t *data, uint16_t len);

    // Read `len` bytes into `buf` from `regAddr` on device `devAddr` (7-bit).
    static bool read(uint8_t devAddr, uint8_t regAddr,
                     uint8_t *buf, uint16_t len);

    // Single-register convenience helpers
    static bool writeReg(uint8_t devAddr, uint8_t regAddr, uint8_t value);
    static uint8_t readReg(uint8_t devAddr, uint8_t regAddr);
};

#endif // I2C_H
