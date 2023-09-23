#include "Config.h"


/*
chỉ giao tiếp SPI1 được 
chưa đọc được giá trị cảm biến gửi về từ board f411
*/



/*
SPI - 1
-->
PA4 --> SS
PA5 --> SCLK
PA6 --> MISO
PA7 --> MOSI
*/
//config theo I3G4250D
void SPI_I3G4250D_Config(){
RCC->APB2ENR |= (1<<12);  // Enable SPI1 CLock
	
	SPI1->CR1 |= (1<<0)|(1<<1);   // CPOL=1, CPHA=1
	
	SPI1->CR1 |= (1<<2);  // Master Mode
	
	SPI1->CR1 |= SPI_CR1_BR_1;  // BR[2:0] = 011: fPCLK/2, PCLK2 = 8MHz, SPI clk = 4MHz
	
	SPI1->CR1 &= ~(1<<7);  // LSBFIRST = 0, MSB first
	
	SPI1->CR1 |= (1<<8) | (1<<9);  // SSM=1, SSi=1 -> Software Slave Management
	
	SPI1->CR1 &= ~(1<<10);  // RXONLY = 0, full-duplex
	
	SPI1->CR1 &= ~(1<<11);  // DFF=0, 8 bit data
//	SPI1->CR1 = SPI_CR1_DFF;  // DFF=1, 16 bit data
	SPI1->CR2 = 0;	
    
        
        RCC->AHB1ENR |= (1<<0);  // Enable GPIO Clock
	
	GPIOA->MODER |= (2<<10)|(2<<12)|(2<<14)|(1<<18);  // Alternate functions for PA5, PA6, PA7 
	
	GPIOA->OSPEEDR |= (3<<10)|(3<<12)|(3<<14)|(3<<18);  // HIGH Speed for PA5, PA6, PA7
	
	GPIOA->AFR[0] |= (5<<20)|(5<<24)|(5<<28);   // AF5(SPI1) for PA5, PA6, PA7
        
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;//clock for port E
        GPIOE->MODER  |= GPIO_MODER_MODER3_0;//Output for PA9
        GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3_1;//Output for PA9
}
void SPI_Enable (void)
{
	SPI1->CR1 |= (1<<6);   // SPE=1, Peripheral enabled
}

void SPI_Disable (void)
{
	SPI1->CR1 &= ~(1<<6);   // SPE=0, Peripheral Disabled
}

void CS_Enable (void)
{
	GPIOA->BSRRH |= (1<<9);
}

void CS_Disable (void)
{
	GPIOA->BSRRL |= (1<<9);
}

void SPI_Transmit (uint8_t *data, int size)
{
	
	/************** STEPS TO FOLLOW *****************
	1. Wait for the TXE bit to set in the Status Register
	2. Write the data to the Data Register
	3. After the data has been transmitted, wait for the BSY bit to reset in Status Register
	4. Clear the Overrun flag by reading DR and SR
	************************************************/		
	
	int i=0;
	while (i<size)
	{
	   while (!((SPI1->SR)&(1<<1))) {};  // wait for TXE bit to set -> This will indicate that the buffer is empty
	   SPI1->DR = data[i];  // load the data into the Data Register
	   i++;
	}	
	
	
/*During discontinuous communications, there is a 2 APB clock period delay between the
write operation to the SPI_DR register and BSY bit setting. As a consequence it is
mandatory to wait first until TXE is set and then until BSY is cleared after writing the last
data.
*/
	while (!((SPI1->SR)&(1<<1))) {};  // wait for TXE bit to set -> This will indicate that the buffer is empty
	while (((SPI1->SR)&(1<<7))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication	
	
	//  Clear the Overrun flag by reading DR and SR
	uint8_t temp = SPI1->DR;
					temp = SPI1->SR;
	
}
void SPI_Receive (uint8_t *data, int size)
{
	/************** STEPS TO FOLLOW *****************
	1. Wait for the BSY bit to reset in Status Register
	2. Send some Dummy data before reading the DATA
	3. Wait for the RXNE bit to Set in the status Register
	4. Read data from Data Register
	************************************************/		

	while (size)
	{
		while (((SPI1->SR)&(1<<7))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
		SPI1->DR = 0;  // send dummy data
		while (!((SPI1->SR) &(1<<0))){};  // Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
	  *data = (SPI1->DR);
	  data++;
		size--;
	}	
}


uint8_t SPI_Send_Byte(uint8_t byteToSend){
    
 while (!((SPI1->SR)&(1<<1))) {};  // wait for TXE bit to set -> This will indicate that the buffer is empty
SPI1->DR = byteToSend;  // load the data into the Data Register
    
   	
/*During discontinuous communications, there is a 2 APB clock period delay between the
write operation to the SPI_DR register and BSY bit setting. As a consequence it is
mandatory to wait first until TXE is set and then until BSY is cleared after writing the last
data.
*/
	while (!((SPI1->SR)&(1<<1))) {};  // wait for TXE bit to set -> This will indicate that the buffer is empty
	while (((SPI1->SR)&(1<<7))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication	
	
	//  Clear the Overrun flag by reading DR and SR
	uint8_t temp = SPI1->DR;
	uint8_t temp2 = SPI1->SR;
        return temp;
	 
}

void SPI_Write_Data(uint8_t address,uint8_t byteToSend){
    
    GPIOE->ODR &= ~GPIO_ODR_ODR_3;
    SPI_Send_Byte(address);
    SPI_Send_Byte(byteToSend);
    GPIOE->ODR |= GPIO_ODR_ODR_3;
    
}

uint8_t SPI_Read_Data(uint8_t address){
    uint8_t temp;
    GPIOE->ODR &= ~GPIO_ODR_ODR_3;
//     accdelay(100);
    SPI_Send_Byte(address);
    temp=SPI_Send_Byte(0);
//    accdelay(100);
    GPIOE->ODR |= GPIO_ODR_ODR_3;
    return temp;
}




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

void int2char(int num, unsigned char str[])
{
char lstr[30];
int cnt = 0;
int div = 10;
int j = 0;

while( num >= div)
{
	lstr[cnt] = num % div + 0x30;
	num /= 10;
	cnt++;
}
	lstr[cnt] = num + 0x30;
for(j= cnt ; j >=0;j--)
{
	str[cnt-j] = lstr[j];
}
str[cnt+1]='\0';
}




int main()
{
    uint8_t data[3] = {0x8f};
    uint8_t data2[10] = {0x20,0xf,};
    uint8_t data21[10] = {0x23,48,};
    
    uint8_t data3[10] = {0,};
    uint8_t data4[10] = {0,};
    uint8_t data5[10] = {0xA0,};
    int16_t value;
    uint8_t value_char[100];
    uint8_t *p= (uint8_t *)&value;

    SysClockConfig ();
    GPIO_Led_Config ();
    TIM2_Config();
    USART2_Config();

    SPI_I3G4250D_Config ();
    SPI_Enable ();

//    delay(1000);
//    GPIOE->ODR &= ~GPIO_ODR_ODR_3;
//    accdelay(10);
//    SPI_Transmit(data2,2);
//    accdelay(10);
//    GPIOE->ODR |= GPIO_ODR_ODR_3;
//    
//    GPIOE->ODR &= ~GPIO_ODR_ODR_3;
//    accdelay(10);
//    SPI_Transmit(data21,2);
//    accdelay(10);
//    GPIOE->ODR |= GPIO_ODR_ODR_3;
//    
//    delay(1000);

//SPI_Write_Data(0x20,0xf);
//SPI_Write_Data(0x24,(1<<6));
 GPIOE->ODR &= ~GPIO_ODR_ODR_3;
 accdelay(10);
SPI_Transmit(data2,2);
accdelay(10);
GPIOE->ODR |= GPIO_ODR_ODR_3;
// delay(1000);
// SPI_Write_Data(0x20,0x0);
 
//SPI_Write_Data(0x20,0x0);
while (1)
{

//data3[0]=SPI_Read_Data(0xA8);
//data3[1]=SPI_Read_Data(0xA9);
    

//    GPIOE->ODR &= ~GPIO_ODR_ODR_3;
//  
//    SPI_Transmit(data2,2);
//    
//    GPIOE->ODR |= GPIO_ODR_ODR_3;
//   accdelay(10); 
//    GPIOE->ODR &= ~GPIO_ODR_ODR_3;
    accdelay(10);
//    SPI_Transmit(data5,1);
//  
//    SPI_Receive(data3,1);
 
 data3[0]=   SPI_Read_Data(0xA0);

    accdelay(10); 
    
    
//    GPIOE->ODR |= GPIO_ODR_ODR_3;

UART2_SendChar(data3[0]);
UART2_SendChar(data3[1]);
//UART2_SendChar(data3[2]);
//UART2_SendChar(data3[3]);
//UART2_SendChar(data3[4]);
//UART2_SendChar(data3[5]);
UART2_SendChar(0xfa);

  
delay(1000);


    }

}

////nên có delay giữa enable và disable -> accdelay(1);
//GPIOE->ODR &= ~GPIO_ODR_ODR_3;
//accdelay(1);
//SPI_Transmit(data,3);
//accdelay(1);
//GPIOE->ODR |= GPIO_ODR_ODR_3;
//GPIOD->ODR |= GPIO_ODR_ODR_12;//Led
//accdelay(1000);


//data3[0]=SPI_Read_Data(0xA8);
//data3[1]=SPI_Read_Data(0xA9);
//
//UART2_SendChar(0x0a);
//UART2_SendChar(data3[0]);
//UART2_SendChar(data3[1]);
//delay(1000);