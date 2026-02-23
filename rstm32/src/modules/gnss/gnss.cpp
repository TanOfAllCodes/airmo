#include "gnss.h"
#include "modules/i2c/i2c.h"

void Gnss::init() {
    // Configure GNSS module over I2C (poll mode, desired message rate, etc.)
}

bool Gnss::read(GnssData &out) {
    (void)out;
    // Read UBX NAV-PVT or NMEA sentences over I2C
    // Parse into GnssData
    return false; // TODO
}
