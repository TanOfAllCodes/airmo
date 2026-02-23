#include "blink4s.h"
#include "target.h"

// STM32F072 Discovery LED Pins (Corrected):
// LD3 (Red)    : PC6
// LD4 (Orange) : PC8
// LD5 (Green)  : PC9
// LD6 (Blue)   : PC7

void Blink4s::init() {
    // Enable clock for GPIOC
    RCC_AHBENR |= RCC_AHBENR_IOPCEN;

    // Configure PC6, PC7, PC8, PC9 as output
    // MODERy[1:0] = 01 (General purpose output mode)
    // Clear bits for PC6-PC9 (bits 12 to 19)
    GPIOC->MODER &= ~(0xFF << 12); 
    // Set bits for Output mode (01 for each pin)
    // (1<<12) | (1<<14) | (1<<16) | (1<<18) -> 0x55 << 12
    GPIOC->MODER |= (0x55 << 12);
}

void Blink4s::toggleAll() {
    // Toggle PC6, PC7, PC8, PC9
    GPIOC->ODR ^= (0xF << 6); 
}

void Blink4s::turnOnAll() {
    // Set PC6, PC7, PC8, PC9 High
    GPIOC->BSRR = (0xF << 6); 
}

void Blink4s::turnOffAll() {
    // Set PC6, PC7, PC8, PC9 Low (Reset bits in BSRR high half)
    GPIOC->BSRR = (0xF << (6 + 16)); 
}

// Individual Control (BSRR = Bit Set/Reset Register)
// Lower 16 bits set the pin (Turn ON)
// Upper 16 bits reset the pin (Turn OFF)

// LD3 = PC6 (Red)
void Blink4s::turnOnLD3() { GPIOC->BSRR = (1 << 6); }
// LD4 = PC8 (Orange)
void Blink4s::turnOnLD4() { GPIOC->BSRR = (1 << 8); }
// LD5 = PC9 (Green)
void Blink4s::turnOnLD5() { GPIOC->BSRR = (1 << 9); }
// LD6 = PC7 (Blue)
void Blink4s::turnOnLD6() { GPIOC->BSRR = (1 << 7); }

void Blink4s::turnOffLD3() { GPIOC->BSRR = (1 << (6 + 16)); } // PC6 Off
void Blink4s::turnOffLD4() { GPIOC->BSRR = (1 << (8 + 16)); } // PC8 Off
void Blink4s::turnOffLD5() { GPIOC->BSRR = (1 << (9 + 16)); } // PC9 Off
void Blink4s::turnOffLD6() { GPIOC->BSRR = (1 << (7 + 16)); } // PC7 Off

void Blink4s::toggleLD3() { GPIOC->ODR ^= (1 << 6); }
void Blink4s::toggleLD4() { GPIOC->ODR ^= (1 << 8); }
void Blink4s::toggleLD5() { GPIOC->ODR ^= (1 << 9); }
void Blink4s::toggleLD6() { GPIOC->ODR ^= (1 << 7); }
