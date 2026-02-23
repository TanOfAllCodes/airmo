#include "thermal_humidity.h"
#include "modules/i2c/i2c.h"

void ThermalHumidity::init() {
    // Send soft-reset, configure measurement mode
}

bool ThermalHumidity::read(ThermalHumidityData &out) {
    (void)out;
    // Trigger measurement, wait, read T + RH registers
    return false; // TODO
}
