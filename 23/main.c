#include "Custom_BSP.h"
#include "miros.h"

#if 0
int main()
{
    SysClockConfig ();
    GPIO_Config ();
    
    SysTick->CTRL = 0b011;
    SysTick->LOAD = 1000000;

//    NVIC_DisableIRQ(PendSV_IRQn);
// __disable_irq();
    while (1);

}
#endif
void main_blinky1(){
   while (1){
    GPIOD_LED_12_ON();
    BSP_delay(SysTick_1s);
    GPIOD_LED_12_OFF();  
     BSP_delay(SysTick_1s);
   }
}

void main_blinky2(){
   while (1){
    GPIOD_LED_13_ON();
    BSP_delay(SysTick_1s);
    GPIOD_LED_13_OFF();  
     BSP_delay(SysTick_1s);
   }
}
uint32_t stack_blinky1[40] ;
OSThread blinky1;


uint32_t stack_blinky2[40];
OSThread blinky2 ;

int main()
{
    volatile static int a =0;
    
    SysClockConfig ();
    GPIO_Config ();
    SysTick_init();
    
      /* fabricate Cortex-M ISR stack frame for blinky1 */
    OSThread_start(&blinky1,
                   &main_blinky1,
                   stack_blinky1, sizeof(stack_blinky1));

    /* fabricate Cortex-M ISR stack frame for blinky2 */
    OSThread_start(&blinky2,
                   &main_blinky2,
                   stack_blinky2, sizeof(stack_blinky2));
    while(1);

}



