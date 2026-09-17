#include "stm32f1xx.h"


__attribute__((noinline)) void delay(volatile unsigned int count) {
    while(count--) {
        __asm__("nop");
    }
}


void SystemInit(void) {
}

int main(void) {
   
    RCC->APB2ENR |= (1 << 4);

    
    GPIOC->CRH &= ~(0xF << 20);   
    GPIOC->CRH |=  (0x2 << 20);   

    while(1) {
        
        GPIOC->ODR &= ~(1 << 13);   
        delay(1000000);             
        
       
        GPIOC->ODR |= (1 << 13);       
        delay(1000000);             
    }
}
