#include"Custom_BSP.h"


static unsigned int l_tick;

void GPIOD_LED_12_ON(){
GPIOD->ODR |= GPIO_ODR_ODR_12;
}
void GPIOD_LED_12_OFF(){
GPIOD->ODR &=~ GPIO_ODR_ODR_12;
}

void GPIOD_LED_13_ON(){
GPIOD->ODR |= GPIO_ODR_ODR_13;
}
void GPIOD_LED_13_OFF(){
GPIOD->ODR &=~ GPIO_ODR_ODR_13;
}

void SysTick_Handler(){
  l_tick++;
}

void SysTick_init(){
    SysTick->CTRL = 0b011;
    SysTick->LOAD = 10000;//100 tick trong 1s
}

uint32_t BSP_tickCtr(){
uint32_t tick;
__disable_irq();
tick = l_tick;
__enable_irq();
return tick;
}

void BSP_delay(uint32_t ticks){
uint32_t start = BSP_tickCtr();

while(BSP_tickCtr() - start < ticks){}
}
