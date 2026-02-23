#ifndef LEDRING_H
#define LEDRING_H

#include "FreeRTOS.h"
#include "task.h"

class LedRing {
public:
    static void init();
    static void start();
    
    // Variable to control delay
    static TickType_t delayValue;
    static TickType_t delayValueInit;

private:
    static void updateLoop();
};

#endif // LEDRING_H
