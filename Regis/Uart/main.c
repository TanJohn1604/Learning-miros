#include "Config.h"


void USART2_Config(){

  RCC->APB1ENR |= RCC_APB1ENR_USART2EN;//bat clock usart  
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;//bat clock port a
  GPIOA->MODER |= GPIO_MODER_MODER2_1;//bat alternate function a2
  GPIOA->MODER |= GPIO_MODER_MODER3_1;//bat alternate function a3
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2;//bat toc do cao cho a2
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3;//bat toc do cao cho a3
  GPIOA->AFR[0] |= (7<<8);//bat alternate function a2 la usart2
  GPIOA->AFR[0] |= (7<<12);//bat alternate function a3 la usart2
 
  USART2->CR1 = 0x00; 
  USART2->CR1 |= USART_CR1_UE;//bat usart2
  USART2->CR1 &= ~(1<<12);//M=0 , 8 bit data
  USART2->CR2 &= ~(0b11<<12);//1 stop bit
  
  USART2->BRR |= (52<<4); //dieu chinh toc do baudrate 9600
  USART2->BRR |= 1; //dieu chinh toc do baudrate 9600
  
  USART2->CR1 |= USART_CR1_TE;  // TE=1.. Enable Transmitter
  USART2->CR1 |= USART_CR1_RE;  // RE=1.. Enable Recei
  
 
}
void UART2_SendChar (uint8_t c)
{
		/*********** STEPS FOLLOWED *************
	
	1. Write the data to send in the USART_DR register (this clears the TXE bit). Repeat this
		 for each data to be transmitted in case of single buffer.
	2. After writing the last data into the USART_DR register, wait until TC=1. This indicates
		 that the transmission of the last frame is complete. This is required for instance when
		 the USART is disabled or enters the Halt mode to avoid corrupting the last transmission.
	
	****************************************/

  USART2->DR = c; // load the data into DR register
    while (!(USART2->SR & (1<<6)));  // Wait for TC to SET.. This indicates that the data has been transmitted

}

void UART2_SendString (uint8_t *string)
{
	while (*string) UART2_SendChar (*string++);
}

uint8_t UART2_GetChar (void)
{
			/*********** STEPS FOLLOWED *************
	
	1. Wait for the RXNE bit to set. It indicates that the data has been received and can be read.
	2. Read the data from USART_DR  Register. This also clears the RXNE bit
	
	****************************************/

	uint8_t temp;
	
	while (!(USART2->SR & USART_SR_RXNE));  // wait for RXNE bit to set
	temp = USART2->DR;  // Read the data. This clears the RXNE also
	return temp;
}


int main()
{
    uint8_t arr[]="abcd";
    uint8_t arr2;
    SysClockConfig ();
    GPIO_Config ();
    TIM2_Config();
    USART2_Config();
    while (1)
    {

    arr2 = UART2_GetChar();
    GPIOD->ODR |= GPIO_ODR_ODR_12;
    accdelay(1000);
    UART2_SendChar(arr2+1);
    GPIOD->ODR &= ~GPIO_ODR_ODR_12;
    accdelay(1000);

    }

}
