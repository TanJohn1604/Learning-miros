#include "Custom_BSP.h"
#include "miros.h"

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

void GPIOD_LED_14_ON(){
GPIOD->ODR |= GPIO_ODR_ODR_14;
}
void GPIOD_LED_14_OFF(){
GPIOD->ODR &=~ GPIO_ODR_ODR_14;
}


void SysTick_Handler(){
    
  l_tick++;
__disable_irq();
OS_sched();
__enable_irq();

  
}

//void PendSV_Handler(){
////  l_tick++;
//}



void SysTick_init(){
    SysTick->CTRL = 0b011;//0b001 sẽ tắt ngắt systick 0b011 sẽ bật
/*  100 tick trong 1s, 
    xung nhịp chíp 8Mhz
    xung nhịp sysTick 1Mhz*/
    SysTick->LOAD = 1000000 / SysTick_1s;
    NVIC_SetPriority(SysTick_IRQn,0);
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
