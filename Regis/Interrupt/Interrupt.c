#include "Interrupt.h"

void GPIO_Config_B1_Interrupt()
{
    	// 1. Enable the GPIO CLOCK
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	// 2. Set the Pin as INPUT
	GPIOA->MODER &= ~GPIO_MODER_MODER0;
	// 3. Configure the INPUT PUSHPULL 
	GPIOA->OTYPER = 0;
	GPIOA->OSPEEDR = 0;
        GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_1;
        //4.Interrupt mask
        EXTI->IMR |= EXTI_IMR_MR0;
        //5. rising
        EXTI->RTSR |= EXTI_RTSR_TR0;
        
        NVIC_EnableIRQ(EXTI0_IRQn);
    __enable_irq();
   
}

void EXTI0_IRQHandler(void) {

 GPIOD->ODR |= GPIO_ODR_ODR_12;
}

