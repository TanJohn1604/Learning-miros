#ifndef __CONFIG_H__
#define __CONFIG_H__
#include "stm32f411xe.h"
void SysClockConfig (void);
void GPIO_Config (void);
void TIM2_Config (void);
void accdelay (uint32_t time);

#endif

