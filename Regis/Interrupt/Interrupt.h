#ifndef INTERRUPT_H
#define INTERRUPT_H
#include "stm32f411xe.h"

void GPIO_Config_B1_Interrupt();
void EXTI1_IRQHandler(void);
#endif