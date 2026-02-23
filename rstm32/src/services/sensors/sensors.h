#ifndef SENSORS_H
#define SENSORS_H

#include "FreeRTOS.h"
#include "task.h"

#include "modules/gnss/gnss.h"
#include "modules/imu/imu.h"
#include "modules/thermal_humidity/thermal_humidity.h"
#include "modules/barometer/barometer.h"
#include "modules/aerosol/aerosol.h"
#include "modules/solar/solar.h"
#include "modules/infrared/infrared.h"

/*
 * Sensors service — single FreeRTOS task that owns the I2C bus
 * and polls every sensor on a configurable cadence.
 *
 * Latest readings are stored in static members for other tasks
 * to consume (read-only; add a mutex if contention arises).
 */

class Sensors {
public:
    static void init();   // I2C + all sensor init
    static void start();  // blocking poll loop (call from task)

    // Latest readings (populated each cycle)
    static GnssData             gnss;
    static ImuData              imu;
    static ThermalHumidityData  thermalHumidity;
    static BarometerData        barometer;
    static AerosolData          aerosol;
    static SolarData            solar;
    static InfraredData         infrared;

    // Poll interval
    static TickType_t pollInterval;
};

#endif // SENSORS_H
