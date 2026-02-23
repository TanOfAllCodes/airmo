#ifndef BLINK4S_H
#define BLINK4S_H

#include <stdint.h>

class Blink4s {
public:
    static void init();
    static void toggleAll();
    static void turnOnAll();
    static void turnOffAll();

    // Individual Control
    static void turnOnLD3(); // Red (PC6)
    static void turnOnLD4(); // Orange (PC8)
    static void turnOnLD5(); // Green (PC9)
    static void turnOnLD6(); // Blue (PC7)

    static void turnOffLD3();
    static void turnOffLD4();
    static void turnOffLD5();
    static void turnOffLD6();

    static void toggleLD3();
    static void toggleLD4();
    static void toggleLD5();
    static void toggleLD6();
};

#endif // BLINK4S_H
