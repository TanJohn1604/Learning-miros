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
//    GPIOD_LED_12_ON();
//    OS_delay(SysTick_1s*2);
//    GPIOD_LED_12_OFF();  
//     OS_delay(SysTick_1s*2);
    
       for(int i=0;i<4500;i++){
       GPIOD_LED_12_ON(); 
    GPIOD_LED_12_OFF();  
       }
   
     OS_delay(7);
       
   }
}

void main_blinky2(){
   while (1){
//    GPIOD_LED_13_ON();
//    OS_delay(SysTick_1s);
//    GPIOD_LED_13_OFF();  
//     OS_delay(SysTick_1s);
     


    for(int i=0;i<10000;i++){
    GPIOD_LED_13_ON();
    GPIOD_LED_13_OFF();  
    }
    
    OS_delay(14);
   }
}

void main_blinky3(){
   while (1){
    GPIOD_LED_14_ON();
    OS_delay(SysTick_1s*1.5);
    GPIOD_LED_14_OFF();  
     OS_delay(SysTick_1s*1.5);
   }
}


uint32_t stack_blinky1[40] ;
OSThread blinky1;


uint32_t stack_blinky2[40];
OSThread blinky2 ;

uint32_t stack_blinky3[40];
OSThread blinky3 ;

uint32_t stack_idleThread[40];
int main()
{
    
    SysClockConfig ();
    GPIO_Config ();
    SysTick_init();
    OS_init(stack_idleThread, sizeof(stack_idleThread));
    
    /* start blinky1 thread */
    OSThread_start(&blinky1,
                   5U, /* priority */
                   &main_blinky1,
                   stack_blinky1, sizeof(stack_blinky1));

    /* start blinky2 thread */
    OSThread_start(&blinky2,
                   6U, /* priority */
                   &main_blinky2,
                   stack_blinky2, sizeof(stack_blinky2));
    /* start blinky3 thread */
    //OSThread_start(&blinky3,
    //               1U, /* priority */
    //               &main_blinky3,
    //               stack_blinky3, sizeof(stack_blinky3));
    OS_run();

}



