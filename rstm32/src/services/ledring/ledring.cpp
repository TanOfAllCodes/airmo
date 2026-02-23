#include "ledring.h"
#include "modules/blink4s/blink4s.h"

// Initialize delayValue
TickType_t LedRing::delayValue = pdMS_TO_TICKS(100);
TickType_t LedRing::delayValueInit = pdMS_TO_TICKS(50);

const int flashing = 10; 

void oneSecDelay(int times)
{
    vTaskDelay(pdMS_TO_TICKS(1000 * times));
}

void LedRing::init() {
    Blink4s::init();
    // Start with all off
    Blink4s::turnOffAll();
    oneSecDelay(1);

    int y;

    for( y=0; y<=flashing;y++)
    {
        Blink4s::turnOffAll();
        vTaskDelay(delayValueInit);
        Blink4s::turnOnAll();
        vTaskDelay(delayValueInit);
    }
    Blink4s::turnOffAll();
    oneSecDelay(1);

}

void LedRing::start() {
    while (1) {

        // Clockwise
        Blink4s::turnOffLD4();
        Blink4s::turnOnLD3();
        vTaskDelay(delayValue);
        Blink4s::turnOffLD3();
        Blink4s::turnOnLD5();
        vTaskDelay(delayValue);
        Blink4s::turnOffLD5();
        Blink4s::turnOnLD6();
        vTaskDelay(delayValue);
        Blink4s::turnOffLD6();
        Blink4s::turnOnLD4();
        vTaskDelay(delayValue);

        // Anti-Clockwise
        Blink4s::turnOffLD4();
        Blink4s::turnOnLD6();
        vTaskDelay(delayValue);
        Blink4s::turnOffLD6();
        Blink4s::turnOnLD5();
        vTaskDelay(delayValue);
        Blink4s::turnOffLD5();
        Blink4s::turnOnLD3();
        vTaskDelay(delayValue);
        Blink4s::turnOffLD3();
        Blink4s::turnOnLD4();
        vTaskDelay(delayValue);
    }
}
