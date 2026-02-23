#ifndef TARGET_H
#define TARGET_H

#include <stdint.h>

// Base Addresses
#define RCC_BASE        0x40021000
#define GPIOC_BASE      0x48000800

// RCC Registers
#define RCC_AHBENR      (*((volatile uint32_t *)(RCC_BASE + 0x14)))
#define RCC_AHBENR_IOPCEN   (1 << 19)

// GPIO Registers
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

#define GPIOC           ((GPIO_TypeDef *) GPIOC_BASE)

#endif // TARGET_H
