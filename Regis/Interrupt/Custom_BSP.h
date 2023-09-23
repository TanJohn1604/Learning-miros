#ifndef CUSTOM_BSP_H
#define CUSTOM_BSP_H
#include "Config.h"
#define SysTick_1s 100
void GPIOD_LED_12_ON();
void GPIOD_LED_12_OFF();

void GPIOD_LED_13_ON();
void GPIOD_LED_13_OFF();

void SysTick_init();
uint32_t BSP_tickCtr();
void BSP_delay(uint32_t ticks);

#endif