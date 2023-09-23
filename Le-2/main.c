#include "stm32f411xe.h"

int main(){
	int i=0;
	RCC->CR |= RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY));
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	
	GPIOD->MODER |= GPIO_MODER_MODER12_0;
	
	GPIOD->PUPDR |= GPIO_PUPDR_PUPDR12_1;
	
	while(1){ 
GPIOD->ODR |= GPIO_ODR_ODR_12;
for(i=0;i<1000000;i++);
GPIOD->ODR &= ~GPIO_ODR_ODR_12;
for(i=0;i<1000000;i++);
	}
	return 1;
}
