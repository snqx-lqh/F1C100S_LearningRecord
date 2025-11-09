#pragma once

#include "f1c100s_gpio.h"

#include <stdint.h>

typedef struct
{
    uint32_t CFG0; // addr 0
    uint32_t CFG1; // addr 4
    uint32_t CFG2; // addr 8
    uint32_t CFG3; // addr 12-0x0c
    uint32_t DATA; // addr 16-0x10
    uint32_t DRV0; // addr 20-0x14
    uint32_t DRV1; // addr 24-0x18
    uint32_t PUL0; // addr 28-0x1c
    uint32_t PUL1; // addr 32-0X20
} GPIO_DEF;

#define GPIOA_PORT ((GPIO_DEF *)(0x01C20800 + 0 * 0X24))
#define GPIOB_PORT ((GPIO_DEF *)(0x01C20800 + 1 * 0X24))
#define GPIOC_PORT ((GPIO_DEF *)(0x01C20800 + 2 * 0X24))
#define GPIOD_PORT ((GPIO_DEF *)(0x01C20800 + 3 * 0X24))
#define GPIOE_PORT ((GPIO_DEF *)(0x01C20800 + 4 * 0X24))
#define GPIOF_PORT ((GPIO_DEF *)(0x01C20800 + 5 * 0X24))
 
#define LED1 0
#define LED2 1

void led_init();
void led_switch(int i, int state);
