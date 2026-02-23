#ifndef TARGET_H
#define TARGET_H

#include <stdint.h>

/*
 * Minimal bare-metal register definitions for STM32H753ZI (Cortex-M7)
 * Nucleo-144 board: LD1 (Green) PB0, LD2 (Blue) PB7, LD3 (Red) PB14
 */

// ── Base Addresses ──────────────────────────────────────────────────────────
#define RCC_BASE        0x58024400
#define GPIOB_BASE      0x58020400

// ── RCC AHB4 Enable Register ────────────────────────────────────────────────
#define RCC_AHB4ENR     (*((volatile uint32_t *)(RCC_BASE + 0xE0)))
#define RCC_AHB4ENR_GPIOBEN  (1 << 1)

// ── GPIO Register Map ───────────────────────────────────────────────────────
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
} GPIO_TypeDef;

#define GPIOB           ((GPIO_TypeDef *) GPIOB_BASE)

// ── Board LED Mapping (Nucleo-144 H753ZI) ───────────────────────────────────
// LD1 (Green) = PB0
// LD2 (Blue)  = PB7
// LD3 (Red)   = PB14

#endif // TARGET_H
