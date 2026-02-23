/*
 * main.cpp
 * FreeRTOS Example for STM32F07x
 */

#include "target.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sensors.h"

#if HAS_RING
#include "ledring.h"
#endif

#if HAS_ETHERNET
#include "telemetry.h"
#endif

// Task handles
#if HAS_RING
TaskHandle_t xLedRingTaskHandle = NULL;
#endif
TaskHandle_t xSensorsTaskHandle = NULL;
#if HAS_ETHERNET
TaskHandle_t xTelemetryTaskHandle = NULL;
#endif

// ----------------------------------------------------------------------------
// Tasks
// ----------------------------------------------------------------------------

#if HAS_RING
void vLedRingTask(void *pvParameters) {
    LedRing::init();
    LedRing::start();
}
#endif

void vSensorsTask(void *pvParameters) {
    Sensors::init();
    Sensors::start();
}

#if HAS_ETHERNET
void vTelemetryTask(void *pvParameters) {
    Telemetry::init();
    Telemetry::start();
}
#endif

// ----------------------------------------------------------------------------
// FreeRTOS Hooks
// ----------------------------------------------------------------------------

extern "C" void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) xTask;
    for( ;; );
}

extern "C" void vApplicationMallocFailedHook( void )
{
    for( ;; );
}

// ----------------------------------------------------------------------------
// Main Application
// ----------------------------------------------------------------------------

void main_app() {
#if HAS_RING
    xTaskCreate(vLedRingTask, "LedRing", 256, NULL, 1, &xLedRingTaskHandle);
#endif

    // Create sensors polling task (512 word stack)
    xTaskCreate(vSensorsTask, "Sensors", 512, NULL, 1, &xSensorsTaskHandle);

#if HAS_ETHERNET
    // Create telemetry TX task (1024 word stack — needs room for packet + TCP buffers)
    xTaskCreate(vTelemetryTask, "Telemetry", 1024, NULL, 3, &xTelemetryTaskHandle);
#endif

    // Start the FreeRTOS scheduler
    vTaskStartScheduler();

    // If we get here, scheduler failed (e.g. heap too small)
    // Hang — board-specific error LED can be added in target code
    while (1);
}

// ----------------------------------------------------------------------------
// Startup Code
// ----------------------------------------------------------------------------

extern "C" uint32_t _estack;
extern "C" uint32_t _sidata; // Changed from _etext
extern "C" uint32_t _sdata;
extern "C" uint32_t _edata;
extern "C" uint32_t _sbss;
extern "C" uint32_t _ebss;

// Forward declaration of handlers (FreeRTOS)
// FreeRTOSConfig.h maps: vPortSVCHandler -> SVC_Handler
//                        xPortPendSVHandler -> PendSV_Handler
//                        xPortSysTickHandler -> SysTick_Handler
extern "C" void SVC_Handler(void);
extern "C" void PendSV_Handler(void);
extern "C" void SysTick_Handler(void);

extern "C" void Default_Handler() {
    while (1);
}

extern "C" void Reset_Handler() {
    uint32_t *pSrc = &_sidata;
    uint32_t *pDest = &_sdata;
    while (pDest < &_edata) *pDest++ = *pSrc++;

    pDest = &_sbss;
    while (pDest < &_ebss) *pDest++ = 0;

    main_app();
    while (1);
}

// Vector Table
__attribute__((section(".isr_vector"), used))
uint32_t * vector_table[] = {
    (uint32_t *)&_estack,
    (uint32_t *)Reset_Handler,      // Reset
    (uint32_t *)Default_Handler,    // NMI
    (uint32_t *)Default_Handler,    // HardFault
    0, 0, 0, 0, 0, 0, 0,            // Reserved
    (uint32_t *)SVC_Handler,        // SVCall  (FreeRTOS)
    0,                              // DebugMon (Reserved)
    0,                              // Reserved
    (uint32_t *)PendSV_Handler,     // PendSV  (FreeRTOS)
    (uint32_t *)SysTick_Handler     // SysTick (FreeRTOS)
};
