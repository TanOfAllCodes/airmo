#include "i2c.h"
#include "target.h"

/*
 * I2C bus driver — STM32 bare-metal.
 *
 * TODO: Implement per-target I2C peripheral init and transfer routines.
 *       The register addresses differ between F07x (I2C1 @ 0x40005400)
 *       and H753 (I2C1 @ 0x40005400, but different clock tree).
 *       Consider adding I2C register definitions to each target.h.
 */

void I2C::init() {
    // 1. Enable I2C1 peripheral clock
    // 2. Configure GPIO pins for I2C (AF, open-drain)
    // 3. Set timing register for desired speed (100kHz / 400kHz)
    // 4. Enable I2C peripheral
}

bool I2C::write(uint8_t devAddr, uint8_t regAddr,
                const uint8_t *data, uint16_t len) {
    (void)devAddr; (void)regAddr; (void)data; (void)len;
    // 1. Wait until bus is free
    // 2. Send START + devAddr (write)
    // 3. Send regAddr
    // 4. Send data bytes
    // 5. Send STOP
    return false; // TODO
}

bool I2C::read(uint8_t devAddr, uint8_t regAddr,
               uint8_t *buf, uint16_t len) {
    (void)devAddr; (void)regAddr; (void)buf; (void)len;
    // 1. Write regAddr (repeated-start)
    // 2. Read `len` bytes into buf
    // 3. NACK last byte, STOP
    return false; // TODO
}

bool I2C::writeReg(uint8_t devAddr, uint8_t regAddr, uint8_t value) {
    return write(devAddr, regAddr, &value, 1);
}

uint8_t I2C::readReg(uint8_t devAddr, uint8_t regAddr) {
    uint8_t val = 0;
    read(devAddr, regAddr, &val, 1);
    return val;
}
