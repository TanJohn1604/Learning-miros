extern int CSTACK$$Limit;
void __iar_program_start(void);
void EXTI0_IRQHandler(void);
void Unused_Handler(void);
extern int CSTACK$$Limit;

int const __vector_table[] @ ".intvec" = {
    (int)&CSTACK$$Limit,
    (int)&__iar_program_start,
    0, 
    0,
    0,
     0, 
    0,
    0,
     0, 
    0,
    0,
    0, 
    0,
    0,
     0, 
    0,
    0,
     0, 
    0,
    0,
(int)&EXTI0_IRQHandler,
};

